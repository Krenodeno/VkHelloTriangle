#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <iostream>
#include <optional>
#include <string>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "RenderTarget.hpp"
#include "RenderUtils.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"

/** Callback */

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT,
	VkDebugUtilsMessageTypeFlagsEXT,
	const VkDebugUtilsMessengerCallbackDataEXT*,
	void*);

/* forward declaration */
class Application;

/* used as callback function to delegate surface creation  */
using createSurfaceFoncter = std::function<vk::SurfaceKHR(Application*, vk::Instance)>;

/* Uniform struct to be removed later */
struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};


class Render {
public:
	Render(std::string appName = "", uint32_t appVersion = 0, RenderType type = RenderTypeBits::eGraphics);
	~Render();

	void init();
	void cleanup();

	void drawFrame();

	void addLayer(const char*);
	void addInstanceExtension(const char*);
	void addDeviceExtension(const char*);

	void enableValidationLayer();

	void setRenderType(RenderType);

	void setParentApplication(Application*);

	void setSurfaceCreationFunction(createSurfaceFoncter);

	void setExtent(const vk::Extent2D&);

	void setAppName(std::string name) { appName = name; }
	void setAppVersion(uint32_t version) { appVersion = version; }

	void waitDeviceIdle() { device.waitIdle(dispatchLoader); }

protected:

	std::string appName;

	uint32_t appVersion;

	RenderType renderType;

	bool validationLayerEnabled;
	std::vector<const char*> layers;
	std::vector<const char*> instanceExtensions;
	std::vector<const char*> deviceExtensions;

	const int MAX_FRAMES_IN_FLIGHT = 2;

	Application* parentApp;

	RenderTarget swapchain;

	createSurfaceFoncter surfaceCreation;

	vk::Extent2D windowExtent;

	vk::DispatchLoaderStatic dispatchLoader;

	vk::Instance instance;

	vk::DebugUtilsMessengerEXT callback;

	vk::PhysicalDevice physicalDevice;

	vk::Device device;

	vk::Queue graphicsQueue;
	vk::Queue computeQueue;

	vk::SurfaceKHR surface;

	vk::Queue presentQueue;

	vk::Image depthImage;
	vk::DeviceMemory depthImageMemory;
	vk::ImageView depthImageView;

	vk::RenderPass renderPass;
	vk::DescriptorSetLayout descriptorSetLayout;
	vk::PipelineLayout pipelineLayout;

	vk::Pipeline graphicsPipeline;

	vk::DescriptorPool descriptorPool;
	std::vector<vk::DescriptorSet> descriptorSets;

	vk::CommandPool commandPool;

	vk::Buffer vertexBuffer;
	vk::DeviceMemory vertexBufferMemory;
	vk::Buffer indexBuffer;
	vk::DeviceMemory indexBufferMemory;

	std::vector<vk::Buffer> uniformBuffers;
	std::vector<vk::DeviceMemory> uniformBuffersMemory;

	std::vector<vk::CommandBuffer> commandBuffers;

	/* Semaphores sync GPU's operations */
	std::vector<vk::Semaphore> imageAvailableSemaphores;
	std::vector<vk::Semaphore> renderFinishedSemaphores;
	/* Fences sync GPU with CPU */
	std::vector<vk::Fence> inFlightFences;
	size_t currentFrame = 0;

	// TODO to be removed
	const std::vector<Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	// initialising functions

	void createInstance();

	void setupDebugCallback();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSurface();

	void createSwapchain(vk::SwapchainKHR = nullptr);

	void createRenderPass();

	void createDescriptorSetLayout();

	void createDescriptorPool();

	void createDescriptorSets();

	void createGraphicsPipeline();

	void createFramebuffers();

	void createCommandPool();

	void createDepthResources();

	void createBuffer(vk::DeviceSize, vk::BufferUsageFlags, vk::MemoryPropertyFlags, vk::Buffer&, vk::DeviceMemory&);

	void createImage(uint32_t width, uint32_t height, vk::Format, vk::ImageTiling, vk::ImageUsageFlags, vk::MemoryPropertyFlags, vk::Image&, vk::DeviceMemory&);

	vk::ImageView createImageView(vk::Image, vk::Format, vk::ImageAspectFlags);

	void transitionImageLayout(vk::Image, vk::Format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout);

	void createVertexBuffer();

	void createIndexBuffer();

	void createUniformBuffers();

	void updateUniformBuffer(uint32_t);

	void copyBuffer(vk::Buffer, vk::Buffer, vk::DeviceSize);

	vk::CommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(vk::CommandBuffer);

	void createCommandBuffers();

	void cleanupSwapchain();

	void recreateSwapChain();

	void createSemaphores();

	void createFences();
};

#endif
