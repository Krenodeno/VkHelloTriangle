#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <iostream>
#include <optional>
#include <string>
#include <functional>
#include <cassert>

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

/* used as callback function to delegate surface creation */
using createSurfaceFoncter = std::function<vk::SurfaceKHR(vk::Instance)>;


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

	/**
	 * Create a buffer of size bytes and return its index
	 * If writeFromHost is true, you can call fillBuffer on it later
	 * IF readFromHost if true, you can call getDataFomBuffer on it later
	 */
	unsigned int addBuffer(uint64_t size, vk::BufferUsageFlags usage, bool writeFromHost = true, bool readFromHost = false);

	/**
	 * Fill a buffer
	 */
	void fillBuffer(unsigned int bufferIndex, const void* data, uint64_t dataSize);

	template<typename T> void fillBuffer(unsigned int bufferIndex, const T& data) {
		fillBuffer(bufferIndex, &data, sizeof(data));
	}

	template<typename T> void fillBuffer(unsigned int bufferIndex, std::vector<T> data) {
		fillBuffer(bufferIndex, data.data(), data.size() * sizeof(data[0]));
	}

	/**
	 * Add a uniform buffer before init time
	 */
	unsigned int addUniform(uint64_t uniformSize, vk::ShaderStageFlags stageFlags) {
		unsigned int uniformIndex = uniformSizes.size();
		uniformSizes.push_back(uniformSize);
		uniformStages.push_back(stageFlags);
		return uniformIndex;
	}

	/**
	 * Update uniform data while rendering
	 */
	void updateUniform(unsigned int uniformIndex, const void* data, uint64_t dataSize) {
		assert(uniformIndex < uniformSizes.size());
		// enlever ça et sauvegarder les données pour faire l'update des uniform buffers après l'acquisition de l'image dans la swapchain
		device.waitForFences(inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max(), dispatchLoader);
		unsigned int buffIndex = uniformIndex * swapchain.getSize() + currentFrame;
		::fillBuffer(device, uniformBuffersMemory[buffIndex], data, dataSize);
	}

	/**
	 * Get data from a buffer
	 */
	template<typename T>
	std::vector<T> getDataFromBuffer(unsigned int index, uint64_t elementCount){
		assert(index < buffers.size());
		uint64_t dataSize = elementCount * sizeof(T);

		// Create staging buffer
		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		createBuffer(dataSize, vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);
		// Copy buffer data from device to host accessbile buffer
		copyBuffer(buffers[index], stagingBuffer, dataSize);

		// Get data back
		auto data = static_cast<T*>(device.mapMemory(stagingBufferMemory, /*offset*/ 0, dataSize));
		std::vector<T> res(data, data + elementCount);
		device.unmapMemory(stagingBufferMemory, dispatchLoader);

		// Destroy staging buffer
		device.freeMemory(stagingBufferMemory, nullptr, dispatchLoader);
		device.destroyBuffer(stagingBuffer, nullptr, dispatchLoader);
		return res;
	}

	void setVertexShader(std::string file) {
		vertexShaderFile = file;
	}

	void setFragmentShader(std::string file) {
		fragmentShaderFile = file;
	}

	void waitForFences() { device.waitForFences(inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max(), dispatchLoader); }

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

	std::string vertexShaderFile;
	std::string fragmentShaderFile;

	vk::Pipeline graphicsPipeline;

	vk::DescriptorPool descriptorPool;
	std::vector<vk::DescriptorSet> descriptorSets;

	vk::CommandPool commandPool;

	std::vector<vk::Buffer> buffers;
	std::vector<vk::DeviceMemory> buffersMemory;
	std::vector<uint64_t> bufferSizes;
	std::vector<vk::BufferUsageFlags> bufferUsages;

	std::vector<uint64_t> uniformSizes;
	std::vector<vk::ShaderStageFlags> uniformStages;
	std::vector<vk::Buffer> uniformBuffers;
	std::vector<vk::DeviceMemory> uniformBuffersMemory;

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

	void createBuffers();

	void createUniformBuffers();

	void updateUniformBuffers(uint32_t currentImage);

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
