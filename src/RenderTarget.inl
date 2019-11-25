#include <vector>

template<typename Dispatch>
void RenderTarget<Dispatch>::init(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::Extent2D windowExtent, vk::SwapchainKHR oldSwapchain, Dispatch d) {

	device = logicalDevice;
	deviceLoader = d;

	std::vector<const char*> extensions = getRequiredExtensions();
	if(!checkDeviceExtensionSupport(physicalDevice, extensions, deviceLoader))
		throw std::runtime_error("Swapchain extension not supported !");

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface, deviceLoader);

	vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities, windowExtent);

	// amount of image in the swapchain
	uint32_t minImageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && minImageCount > swapChainSupport.capabilities.maxImageCount) {
		minImageCount = swapChainSupport.capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.surface = surface;
	createInfo.minImageCount = minImageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; // 2 if stereoscopic is wanted, 1 in others cases
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment; // render image and present it directly

	// Use the right queue
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, deviceLoader);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	// We use Concurrent mode, but Exclusive give more performance but require to be explicit about image sharing
	if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	// Use opaque unless you want to blend background from other windows
	createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = oldSwapchain;

	swapChain = device.createSwapchainKHR(createInfo, nullptr, deviceLoader);

	// Retrieve swapchain images
	swapChainImages = device.getSwapchainImagesKHR(swapChain, deviceLoader);
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
	imageCount = swapChainImages.size();

	createImageViews();
}

template<typename Dispatch>
void RenderTarget<Dispatch>::cleanup() {
	for (auto imageView : swapChainImageViews) {
		device.destroyImageView(imageView, nullptr, deviceLoader);
	}
	swapChainImageViews.clear();
	if (swapChain)
		device.destroySwapchainKHR(swapChain, nullptr, deviceLoader);
}

template<typename Dispatch>
void RenderTarget<Dispatch>::recreate(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, vk::Extent2D windowExtent) {
	// Destroy imageViews
	for (auto imageView : swapChainImageViews) {
		device.destroyImageView(imageView, nullptr, deviceLoader);
	}

	// Recreate swapchain with new extent
	vk::SwapchainKHR oldSwapchain = swapChain;
	init(surface, physicalDevice, device, windowExtent, oldSwapchain, deviceLoader);

	// Destroy old swapchain
	device.destroySwapchainKHR(oldSwapchain, nullptr, deviceLoader);
}

template<typename Dispatch>
std::vector<const char*> RenderTarget<Dispatch>::getRequiredExtensions() {
	return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}

template<typename Dispatch>
vk::SwapchainKHR RenderTarget<Dispatch>::getSwapchain() {
	return swapChain;
}

template<typename Dispatch>
vk::Format RenderTarget<Dispatch>::getImageFormat() {
	return swapChainImageFormat;
}

template<typename Dispatch>
vk::Extent2D RenderTarget<Dispatch>::getExtent() {
	return swapChainExtent;
}

template<typename Dispatch>
unsigned int RenderTarget<Dispatch>::getImageCount() {
	return imageCount;
}

template<typename Dispatch>
vk::ImageView RenderTarget<Dispatch>::getImageView(unsigned int index) {
	return swapChainImageViews[index];
}

template<typename Dispatch>
void RenderTarget<Dispatch>::createImageViews() {
	swapChainImageViews.resize(imageCount);

	for (size_t i = 0; i < swapChainImageViews.size(); ++i) {
		vk::ImageViewCreateInfo createInfo;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = vk::ImageViewType::e2D;
		createInfo.format = swapChainImageFormat;
		// components swizzle
		createInfo.components.r = vk::ComponentSwizzle::eIdentity;
		createInfo.components.g = vk::ComponentSwizzle::eIdentity;
		createInfo.components.b = vk::ComponentSwizzle::eIdentity;
		//
		createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		swapChainImageViews[i] = device.createImageView(createInfo, nullptr, deviceLoader);
	}
}
