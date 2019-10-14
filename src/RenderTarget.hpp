#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include "Vulkan.hpp"

#include <vector>

#include "RenderUtils.hpp"

template<typename Dispatch = vk::DispatchLoaderDefault>
class RenderTarget {
public:

	RenderTarget() {}
	~RenderTarget() {}

	void init(vk::SurfaceKHR, vk::PhysicalDevice, vk::Device, vk::Extent2D, vk::SwapchainKHR = nullptr, Dispatch = Dispatch());
	void cleanup();
	void recreate(vk::SurfaceKHR, vk::PhysicalDevice, vk::Extent2D);

	std::vector<const char*> getRequiredExtensions();

	vk::SwapchainKHR getSwapchain();

	vk::Format getImageFormat();
	vk::Extent2D getExtent();

	vk::Framebuffer getFramebuffer(unsigned long);

	unsigned int getImageCount();

	void createFramebuffers(vk::RenderPass, vk::ImageView depth = nullptr);

private:

	vk::Device device;
	Dispatch deviceLoader;

	vk::SwapchainKHR swapChain;
	unsigned int imageCount;
	std::vector<vk::Image> swapChainImages;
	vk::Format swapChainImageFormat;
	vk::Extent2D swapChainExtent;
	std::vector<vk::ImageView> swapChainImageViews;
	std::vector<vk::Framebuffer> swapChainFramebuffers;

	void createImageViews();

};

#include "RenderTarget.inl"

#endif
