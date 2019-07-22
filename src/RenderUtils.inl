template<typename Dispatch>
bool checkExtensionSupport(const char* extensionName, const Dispatch& d) {
	bool result = false;
	for (auto extension : vk::enumerateInstanceExtensionProperties(nullptr, d)) {
		if (strcmp(extensionName, extension.extensionName) == 0) {
			result = true;
			break;
		}
	}
	return result;
}

template<typename Dispatch>
bool checkLayerSupport(const char* layerName, const Dispatch& d) {
	bool result = false;
	for (auto layer : vk::enumerateInstanceLayerProperties(d)) {
		if (strcmp(layerName, layer.layerName)) {
			result = true;
			break;
		}
	}
	return result;
}

template<typename Dispatch>
bool checkDeviceExtensionSupport(vk::PhysicalDevice device, std::vector<const char*> deviceExtensions, const Dispatch& d) {
	auto availableExtensions = device.enumerateDeviceExtensionProperties(nullptr, d);

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

template<typename Dispatch>
QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, const Dispatch& d) {
	QueueFamilyIndices indices;

	auto queueFamilyProperties = device.getQueueFamilyProperties(d);

	int i = 0;
	for (const auto& queueFamily : queueFamilyProperties) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
			indices.computeFamily = i;
		}

		if (surface) {
			auto presentSupport = device.getSurfaceSupportKHR(i, surface, d);
			if (queueFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
			}
		}

		if (indices.isComplete())
			break;

		++i;
	}
	// return lastly found indices for each families if one is not found
	return indices;
}

template<typename Dispatch>
SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface, const Dispatch& d) {
	SwapChainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface, d);

	details.formats = device.getSurfaceFormatsKHR(surface, d);

	details.presentModes = device.getSurfacePresentModesKHR(surface, d);

	return details;
}

template<typename Dispatch>
bool isDeviceSuitable(vk::PhysicalDevice device, std::vector<const char*> extensions, vk::QueueFlags wantedQueues, vk::SurfaceKHR surface, const Dispatch& d) {
	QueueFamilyIndices indices = findQueueFamilies(device, surface, d);

	bool extensionsSupported = checkDeviceExtensionSupport(device, extensions, d);

	bool swapChainAdequate = true;
	if (extensionsSupported) {
		if (surface) {
			SwapChainSupportDetails swapchainSupport = querySwapChainSupport(device, surface, d);
			swapChainAdequate = !swapchainSupport.formats.empty() && !swapchainSupport.presentModes.empty();
		}
	}

	bool isComplete = true;

	if ((wantedQueues & vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics)
		isComplete &= indices.graphicsFamily.has_value();

	if ((wantedQueues & vk::QueueFlagBits::eCompute) == vk::QueueFlagBits::eCompute)
		isComplete &= indices.computeFamily.has_value();

	if (surface)
		isComplete &= indices.presentFamily.has_value();

	vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures(d);

	return isComplete && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
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
	for (const auto& availablePresentMode : availablePresentModes) {
		// Nice mode for triple-buffering. If the swapchain is full, the images
		// are simply replaced with the newer ones.
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		}
	}
	// Mostly a V-Sync mode, where the swapchain is a queue. If it is full, the application has to wait.
	return vk::PresentModeKHR::eFifo;

	// The other modes are:
	// - Immediate : images submitted are transferred to the screen right away, may result in tearing
	// - FIFO relaxed : if the application is late and the queue was empty, work like immediate in this case, FIFO otherwise
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

template<typename Dispatch>
uint32_t findMemoryType(vk::PhysicalDevice device, uint32_t typeFilter, vk::MemoryPropertyFlags properties, const Dispatch& d) {
	auto memProperties = device.getMemoryProperties(d);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

template<typename Dispatch>
vk::Format findSupportedFormat(vk::PhysicalDevice device, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features, const Dispatch& d) {
	for (auto format : candidates) {
		vk::FormatProperties props = device.getFormatProperties(format, d);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
			return format;
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
			return format;
	}

	throw std::runtime_error("Failed to find supported format!");
}

template<typename Dispatch>
vk::Format findDepthFormat(vk::PhysicalDevice device, const Dispatch& d) {
	return findSupportedFormat( device,
								{vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
								vk::ImageTiling::eOptimal,
								vk::FormatFeatureFlagBits::eDepthStencilAttachment,
								d);
}

bool hasStencilComponent(vk::Format format) {
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

template<typename Dispatch>
vk::ImageView createImageView(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspect, uint32_t mipLevels, const Dispatch& d) {
	vk::ImageViewCreateInfo createInfo;
	createInfo.image = image;
	createInfo.viewType = vk::ImageViewType::e2D;
	createInfo.format = format;
	// components swizzle
	createInfo.components.r = vk::ComponentSwizzle::eIdentity;
	createInfo.components.g = vk::ComponentSwizzle::eIdentity;
	createInfo.components.b = vk::ComponentSwizzle::eIdentity;
	//
	createInfo.subresourceRange.aspectMask = aspect;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = mipLevels;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	return device.createImageView(createInfo, nullptr, d);
}

template<typename Dispatch>
void fillBuffer(vk::Device device, vk::DeviceMemory& memory, const void* dataToCopy, vk::DeviceSize size, const Dispatch& d) {
	auto data = device.mapMemory(memory, /*offset*/ 0, size, vk::MemoryMapFlags(), d);
		std::memcpy(data, dataToCopy, static_cast<size_t>(size));
	device.unmapMemory(memory, d);
}

template<typename T, typename Dispatch>
void fillBuffer(vk::Device device, vk::DeviceMemory& memory, std::vector<T> v, const Dispatch& d) {
	assert(!v.empty());
	vk::DeviceSize bufferSize = sizeof(v[0]) * v.size();
	fillBuffer(device, memory, v.data(), bufferSize, d);
}
