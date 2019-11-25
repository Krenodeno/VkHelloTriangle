#ifndef RENDER_TARGET_HPP
#define RENDER_TARGET_HPP

#include "Vulkan.hpp"

#include <vector>

#include "RenderUtils.hpp"

template<typename Dispatch>
class RenderTarget {
public:

	RenderTarget() {}
	~RenderTarget() { cleanup(); }

	void init(vk::SurfaceKHR, vk::PhysicalDevice, vk::Device, vk::Extent2D, vk::SwapchainKHR = nullptr, Dispatch = Dispatch());
	void cleanup();
	void recreate(vk::SurfaceKHR, vk::PhysicalDevice, vk::Extent2D);

	std::vector<const char*> getRequiredExtensions();

	vk::SwapchainKHR getSwapchain();

	vk::Format getImageFormat();
	vk::Extent2D getExtent();

	unsigned int getImageCount();

	vk::ImageView getImageView(unsigned int index);

private:

	vk::Device device;
	Dispatch deviceLoader;

	vk::SwapchainKHR swapChain;
	unsigned int imageCount;
	std::vector<vk::Image> swapChainImages;
	vk::Format swapChainImageFormat;
	vk::Extent2D swapChainExtent;
	std::vector<vk::ImageView> swapChainImageViews;

	void createImageViews();

};

#include "RenderTarget.inl"

#endif
