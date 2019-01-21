
bool checkExtensionSupport(const char* extensionName) {
	bool result = false;
	for (auto extension : vk::enumerateInstanceExtensionProperties(std::string())) {
		if (strcmp(extensionName, extension.extensionName) == 0) {
			result = true;
			break;
		}
	}
	return result;
}

bool checkLayerSupport(const char* layerName) {
	bool result = false;
	for (auto layer : vk::enumerateInstanceLayerProperties()) {
		if (strcmp(layerName, layer.layerName)) {
			result = true;
			break;
		}
	}
	return result;
}

bool checkDeviceExtensionSupport(vk::PhysicalDevice device, std::vector<const char*> deviceExtensions) {
	auto availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface) {
	QueueFamilyIndices indices;

	auto queueFamilyProperties = device.getQueueFamilyProperties();

	int i = 0;
	for (const auto& queueFamily : queueFamilyProperties) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
			indices.computeFamily = i;
		}

		auto presentSupport = device.getSurfaceSupportKHR(i, surface);
		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete())
			break;

		++i;
	}

	return indices;
}

SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface) {
	SwapChainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);

	details.formats = device.getSurfaceFormatsKHR(surface);

	details.presentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}

bool isDeviceSuitable(vk::PhysicalDevice device, vk::SurfaceKHR surface, std::vector<const char*> extensions) {
	QueueFamilyIndices indices = findQueueFamilies(device, surface);

	bool extensionsSupported = checkDeviceExtensionSupport(device, extensions);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapchainSupport = querySwapChainSupport(device, surface);
		swapChainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
	// Select format ourself as there is not prefered format from the surface
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
	}
	// Let's try to see if the format we want is in the list
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return availableFormat;
		}
	}
	// No choice, take the first format available
	return availableFormats[0];
}

vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
	// Guaranted to be present, but still not really the best
	vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;
	for (const auto& availablePresentMode : availablePresentModes) {
		// Nice mode working as triple-buffering
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		}
		// send image immediatly when available to the screen
		else if (availablePresentMode == vk::PresentModeKHR::eImmediate) {
			bestMode = availablePresentMode;
		}
	}
	return bestMode;
}

vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, vk::Extent2D windowExtent) {
	// If current extent value is set with uint32_t limit value, then the DM
	// tells us that it accept different extent that the window
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		vk::Extent2D actualExtent = windowExtent;

		actualExtent.width = std::max(capabilities.minImageExtent.width,
							 std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height,
							  std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

uint32_t findMemoryType(vk::PhysicalDevice device, uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
	auto memProperties = device.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

vk::Format findSupportedFormat(vk::PhysicalDevice device, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
	for (auto format : candidates) {
		vk::FormatProperties props = device.getFormatProperties(format);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
			return format;
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
			return format;
	}

	throw std::runtime_error("Failed to find supported format!");
}

vk::Format findDepthFormat(vk::PhysicalDevice device) {
	return findSupportedFormat( device,
								{vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
								vk::ImageTiling::eOptimal,
								vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

bool hasStencilComponent(vk::Format format) {
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

void fillBuffer(vk::Device device, vk::DeviceMemory& memory, const void* dataToCopy, vk::DeviceSize size) {
	auto data = device.mapMemory(memory, /*offset*/ 0, size, vk::MemoryMapFlags());
		std::memcpy(data, dataToCopy, static_cast<size_t>(size));
	device.unmapMemory(memory);
}

template<typename T>
void fillBuffer(vk::Device device, vk::DeviceMemory& memory, std::vector<T> v) {
	vk::DeviceSize bufferSize = sizeof(v[0]) * v.size();
	fillBuffer(device, memory, v.data(), bufferSize);
}
