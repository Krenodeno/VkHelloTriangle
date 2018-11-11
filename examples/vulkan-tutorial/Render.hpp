#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <iostream>
#include <optional>
#include <string>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "Shader.hpp"

/** Callback */

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT,
	VkDebugUtilsMessageTypeFlagsEXT,
	const VkDebugUtilsMessengerCallbackDataEXT*,
	void*);

/** structs */

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

/* forward declaration */
class Application;

/* used as callback function to delegate surface creation  */
using createSurfaceFoncter = std::function<vk::SurfaceKHR(Application*, vk::Instance)>;

class Render {
public:
	Render();
	~Render();

	void init();
	void cleanup();

	void drawFrame();

	void addLayer(const char*);
	void addInstanceExtension(const char*);
	void addDeviceExtension(const char*);

	void enableValidationLayer();

	void setParentApplication(Application*);

	void setSurfaceCreationFunction(createSurfaceFoncter);

	void setExtent(const vk::Extent2D&);

	void waitDeviceIdle() { device.waitIdle(dispatchLoader); }

private:

	bool validationLayerEnabled;
	std::vector<const char*> layers;
	std::vector<const char*> instanceExtensions;
	std::vector<const char*> deviceExtensions;

	const int MAX_FRAMES_IN_FLIGHT = 2;

	Application* parentApp;

	createSurfaceFoncter surfaceCreation;

	vk::Extent2D windowExtent;

	vk::DispatchLoaderStatic dispatchLoader;

	vk::Instance instance;

	vk::DebugUtilsMessengerEXT callback;

	vk::PhysicalDevice physicalDevice;

	vk::Device device;

	vk::Queue graphicsQueue;

	vk::SurfaceKHR surface;

	vk::Queue presentQueue;

	vk::SwapchainKHR swapChain;
	std::vector<vk::Image> swapChainImages;
	vk::Format swapChainImageFormat;
	vk::Extent2D swapChainExtent;

	std::vector<vk::ImageView> swapChainImageViews;


	Shader vert, frag;

	vk::RenderPass renderPass;
	vk::PipelineLayout pipelineLayout;

	vk::Pipeline graphicsPipeline;

	std::vector<vk::Framebuffer> swapChainFramebuffers;

	vk::CommandPool commandPool;

	std::vector<vk::CommandBuffer> commandBuffers;

	/* Semaphores sync GPU's operations */
	std::vector<vk::Semaphore> imageAvailableSemaphores;
	std::vector<vk::Semaphore> renderFinishedSemaphores;
	/* Fences sync GPU with CPU */
	std::vector<vk::Fence> inFlightFences;
	size_t currentFrame = 0;


	// initialising functions

	void createInstance();

	void setupDebugCallback();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSurface();

	void createSwapchain();

	void createImageViews();

	void createRenderPass();

	void createGraphicsPipeline();

	void createFrameBuffers();

	void createCommandPool();

	void createCommandBuffers();

	void createSemaphores();

	void createFences();

	// Tools functions

	bool checkExtensionSupport(const char*);
	bool checkLayerSupport(const char*);

	bool isDeviceSuitable(vk::PhysicalDevice);

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice);

	bool checkDeviceExtensionSupport(vk::PhysicalDevice);

	SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice);

	vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>&);

	vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>&);

	vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR&);
};

#endif
