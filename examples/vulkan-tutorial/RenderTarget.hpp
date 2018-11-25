#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include <vulkan/vulkan.hpp>

#include <vector>

#include "RenderUtils.hpp"

class RenderTarget {
public:

	RenderTarget() {}
	~RenderTarget() {}

	void init(vk::SurfaceKHR, vk::PhysicalDevice, vk::Device, vk::Extent2D);
	void cleanup();

	std::vector<const char*> getRequiredExtensions();

	vk::SwapchainKHR getSwapchain();

	vk::Format getImageFormat();
	vk::Extent2D getExtent();

	vk::Framebuffer getFramebuffer(unsigned long);

	unsigned long getSize();

	void createFramebuffers(vk::RenderPass);

private:

	vk::Device device;

	vk::SwapchainKHR swapChain;
	std::vector<vk::Image> swapChainImages;
	vk::Format swapChainImageFormat;
	vk::Extent2D swapChainExtent;
	std::vector<vk::ImageView> swapChainImageViews;
	std::vector<vk::Framebuffer> swapChainFramebuffers;

	void createImageViews();

};

#endif
