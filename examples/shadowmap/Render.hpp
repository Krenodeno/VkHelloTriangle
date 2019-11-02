#ifndef RENDER_HPP
#define RENDER_HPP

#include "Buffer.hpp"
#include "RenderUtils.hpp"
#include "RenderTarget.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"

/* used as callback function to delegate surface creation */
using createSurfaceFoncter = std::function<vk::SurfaceKHR(vk::Instance, vk::DispatchLoaderDynamic)>;

class Render {
public:
	Render(const std::string& appName = "", uint32_t appVersion = 0u, RenderType type = RenderTypeBits::eGraphics);
	~Render();

	void setAppName(const std::string&);
	void setAppVersion(uint32_t);
	void setExtent(const vk::Extent2D&);
	void setRenderType(RenderType);
	void setSurfaceCreationFunction(createSurfaceFoncter);
	void enableValidationLayer();

	void addInstanceExtension(const char*);
	void addDeviceExtension(const char*);
	void addLayer(const char*);

	void setVertexBufferSize(uint64_t size) { vertexBuffer.size = size; }
	void setIndexBufferSize(uint64_t size) { indexBuffer.size = size; }
	void setTextureSize(uint32_t width, uint32_t height) { setTextureSize(width, height, static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1); }
	void setTextureSize(uint32_t width, uint32_t height, uint32_t mipLevels) {
		texture.width = width;
		texture.height = height;
		texture.mipLevels = mipLevels;
	}

	void waitDeviceIdle();
	void waitForFences();

	void init();
	void drawFrame();

	const Buffer& getVertexBuffer() {return vertexBuffer;}
	const Buffer& getIndexBuffer() {return indexBuffer;}
	void fillBuffer(Buffer buffer, const void* data, uint64_t dataSize);
	void fillTexture(void*);

private:

	std::string appName;
	uint32_t appVersion;
	RenderType renderType;
	bool validationLayerEnabled;

	void cleanup();

	VulkanLoader loader;

	std::vector<const char*> layers;
	std::vector<const char*> instanceExtensions;

	vk::UniqueInstance instance;
	vk::DispatchLoaderDynamic instanceLoader;

	vk::UniqueDebugUtilsMessengerEXT messenger;

	vk::PhysicalDevice physicalDevice;

	std::vector<const char*> deviceExtensions;
	std::vector<vk::PhysicalDeviceFeatures> deviceFeatures;

	vk::UniqueHandle<vk::Device , vk::DispatchLoaderDynamic> device;
	vk::DispatchLoaderDynamic deviceLoader;

	vk::Queue graphicQueue;
	vk::Queue computeQueue;

	vk::Queue presentQueue;

	createSurfaceFoncter surfaceCreation;
	vk::Extent2D windowExtent;

	vk::UniqueHandle<vk::SurfaceKHR , vk::DispatchLoaderDynamic> surface;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	RenderTarget<vk::DispatchLoaderDynamic> swapchain;
	uint32_t currentFrame = 0u;

	std::array<Pipeline, 2> pipelines;

	Buffer vertexBuffer = {nullptr, nullptr, vk::BufferUsageFlagBits::eVertexBuffer, 0u, 0u};
	Buffer indexBuffer = {nullptr, nullptr, vk::BufferUsageFlagBits::eIndexBuffer, 0u, 0u};
	Texture texture;

	struct MVPUniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	std::vector<Buffer> sunMVPUniformBuffers;
	std::vector<Buffer> viewMVPUniformBuffers;

	struct LightUniformBufferObject {
		glm::mat4 light;
		glm::vec4 viewWorldPos;
		glm::vec4 lightWorldPos;
	};

	std::vector<Buffer> lightUniformBuffers;


	vk::UniqueSampler textureSampler;

	vk::UniqueCommandPool commandPool;

	std::vector<vk::UniqueCommandBuffer> commandBuffers;

	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;

	std::vector<vk::UniqueFence> inFlightFences;

	// Initialisation process functions

	void createInstance();
	void setupDebugCallback();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSurface();

	void createSwapchain(vk::SwapchainKHR = nullptr);

	void createRenderPass();

	void createDescriptorSetLayout();

	void createGraphicsPipeline();

	void createCommandPool();

	void createTexture();

	void createTextureView();

	Texture createDepthResources(uint32_t width, uint32_t height, vk::ImageUsageFlags = vk::ImageUsageFlags());

	void createFramebuffers();

	void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& memory);

	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory);

	void createTextureSampler(float maxAnisotropy, float maxMipLod);

	void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels);

	void createBuffers();

	void createUniformBuffers();

	void createUniformBuffer(vk::DeviceSize bufferSize, vk::Buffer& buffer, vk::DeviceMemory& memory);

	void createDescriptorPool();

	void createDescriptorSets();

	void copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size);

	void copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height);

	void generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	vk::CommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(vk::CommandBuffer);

	void createCommandBuffers();

	void createSemaphores();

	void createFences();

};

#endif