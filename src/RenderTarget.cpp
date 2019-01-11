#include "RenderTarget.hpp"

#include <vector>

void RenderTarget::init(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::Extent2D windowExtent, vk::SwapchainKHR oldSwapchain) {

	device = logicalDevice;

	std::vector<const char*> extensions = getRequiredExtensions();
	if(!checkDeviceExtensionSupport(physicalDevice, extensions))
		throw std::runtime_error("Swapchain extension not supported !");

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);

	vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities, windowExtent);

	// amount of image in the swapchain
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; // 2 if stereoscopic is wanted, 1 in others cases
	createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment; // render image and present it directly

	// Use the right queue
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
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

	swapChain = device.createSwapchainKHR(createInfo);

	// Retrieve swapchain images
	swapChainImages = device.getSwapchainImagesKHR(swapChain);
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;

	createImageViews();
}



void RenderTarget::cleanup() {
	for (auto framebuffer : swapChainFramebuffers) {
		device.destroyFramebuffer(framebuffer);
	}
	for (auto imageView : swapChainImageViews) {
		device.destroyImageView(imageView);
	}
	device.destroySwapchainKHR(swapChain);
}

void RenderTarget::recreate(vk::SurfaceKHR surface, vk::PhysicalDevice physicalDevice, vk::Extent2D windowExtent) {
	// Destroy framebuffer and imageViews
	for (auto framebuffer : swapChainFramebuffers) {
		device.destroyFramebuffer(framebuffer);
	}
	for (auto imageView : swapChainImageViews) {
		device.destroyImageView(imageView);
	}

	// Recreate swapchain with new extent
	vk::SwapchainKHR oldSwapchain = swapChain;
	init(surface, physicalDevice, device, windowExtent, oldSwapchain);

	// Destroy old swapchain
	device.destroySwapchainKHR(oldSwapchain);
}

std::vector<const char*> RenderTarget::getRequiredExtensions() {
	return { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}

vk::SwapchainKHR RenderTarget::getSwapchain() {
	return swapChain;
}

vk::Format RenderTarget::getImageFormat() {
	return swapChainImageFormat;
}

vk::Extent2D RenderTarget::getExtent() {
	return swapChainExtent;
}

vk::Framebuffer RenderTarget::getFramebuffer(unsigned long i) {
	return swapChainFramebuffers[i];
}

unsigned long RenderTarget::getSize() {
	return swapChainImageViews.size();
}

void RenderTarget::createFramebuffers(vk::RenderPass renderPass, vk::ImageView depthImageView) {
	swapChainFramebuffers.resize(swapChainImageViews.size());

	for(size_t i = 0; i < swapChainImageViews.size(); ++i) {
		std::array<vk::ImageView, 2> attachments = {
			swapChainImageViews[i],
			depthImageView
		};

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		swapChainFramebuffers[i] = device.createFramebuffer(framebufferInfo);
	}
}

void RenderTarget::createImageViews() {
	swapChainImageViews.resize(swapChainImages.size());

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

		swapChainImageViews[i] = device.createImageView(createInfo);
	}
}
