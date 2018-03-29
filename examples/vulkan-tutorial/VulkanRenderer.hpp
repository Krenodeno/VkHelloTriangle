#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "VDeleter.hpp"
#include "Vertex.hpp"
#include "Orbiter.hpp"
#include "FlyCamera.hpp"

#include <VulkanLoader.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

class VulkanApplication;
void createSurface(VulkanApplication* app, VkInstance instance, VkSurfaceKHR * surface);

/******************** UNIFORM BUFFER OBJECT STRUCT ********************/

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

/******************** QUEUE FAMILY STRUCT ********************/

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

/******************** SWAPCHAIN DETAILS STRUCT ********************/

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

/******************** LAYERS AND EXTENSIONS ********************/

const std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

/******************** DEBUG SETTING ********************/

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

/* Callback Helper functions*/

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData);

/******************** APP CLASS ********************/

class VulkanRenderer
{
public:

	void init();
	void recreateSwapChain();
	void updateUniformBuffer();
	void drawFrame();
	void waitIdle();

	VulkanRenderer() : cam(Orbiter(8.0f, glm::radians(45.0f), glm::radians(45.0f), glm::radians(50.0f), 4.0f / 3.0f)) {}

	void addExtension(const char* extensionName);
	void addExtensions(std::vector<const char*> extensionNames);

	void setCreateSurfaceFunction(std::function<void(VulkanApplication*, VkInstance, VkSurfaceKHR*)> createSurface);
	void setParentApplication(VulkanApplication* application);

	void setWidth(int w);
	void setHeight(int h);

	/** Getter */

	VkInstance getInstance();

	VkSurfaceKHR getSurface();

	/* camera functions proxy */

	void move(float step);
	void translate(glm::vec3 translation);
	void rotate(glm::vec2 rotation);

private:

	Orbiter cam;
	FlyCamera fly;

	int test = 0, res = 0;

	int width, height;

	VulkanLoader loader;
	
	std::vector<const char*> requiredExtensions;

	std::function<void(VulkanApplication*, VkInstance, VkSurfaceKHR*)> createSurfaceFunction;

	VulkanApplication* app;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	/** Instance and Surface */
	VDeleter<VkInstance> instance{ vkDestroyInstance };
	VDeleter<VkDebugReportCallbackEXT> callback{ instance, vkDestroyDebugReportCallbackEXT };
	VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VDeleter<VkDevice> device{ vkDestroyDevice };

	VDeleter<VkSwapchainKHR> swapChain{ device, vkDestroySwapchainKHR };
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VDeleter<VkImageView>> swapChainImageViews;

	VDeleter<VkDescriptorSetLayout> descriptorSetLayout{ device, vkDestroyDescriptorSetLayout };
	VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
	VDeleter<VkRenderPass> renderPass{ device, vkDestroyRenderPass };
	VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };

	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool> commandPool{ device, vkDestroyCommandPool };

	VDeleter<VkBuffer> vertexBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> vertexBufferMemory{ device, vkFreeMemory };
	VDeleter<VkBuffer> indexBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> indexBufferMemory{ device, vkFreeMemory };

	VDeleter<VkImage> textureImage{ device, vkDestroyImage };
	VDeleter<VkDeviceMemory> textureImageMemory{ device, vkFreeMemory };
	VDeleter<VkImageView> textureImageView{ device, vkDestroyImageView };
	VDeleter<VkSampler> textureSampler{ device, vkDestroySampler };

	VDeleter<VkImage> depthImage{ device, vkDestroyImage };
	VDeleter<VkDeviceMemory> depthImageMemory{ device, vkFreeMemory };
	VDeleter<VkImageView> depthImageView{ device, vkDestroyImageView };

	VDeleter<VkBuffer> uniformStagingBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> uniformStagingBufferMemory{ device, vkFreeMemory };
	VDeleter<VkBuffer> uniformBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> uniformBufferMemory{ device, vkFreeMemory };

	VDeleter<VkDescriptorPool> descriptorPool{ device, vkDestroyDescriptorPool };
	VkDescriptorSet descriptorSet;

	std::vector<VkCommandBuffer> commandBuffers;

	VDeleter<VkSemaphore> imageAvailableSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkSemaphore> renderFinishedSemaphore{ device, vkDestroySemaphore };

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	/** Member functions */
	void initiateVulkanLib();
	void createInstance();
	void setupDebugCallback();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void getQueues();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createCommandPool();
	void createDepthResources();
	void createFramebuffers();
	void createTextureImage(const std::string texPath);
	void createTextureImageView();
	void createTextureSampler();
	void loadModel(const std::string modelPath);
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffer();
	void createDescriptorPool();
	void createDescriptorSet();
	void createCommandBuffers();
	void createSemaphore();

	/** Helper functions */
	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);

	bool checkExtensionSupport();

	bool checkValidationLayerSupport();

	std::vector<const char*> getRequiredExtensions();

	bool isDeviceSuitable(VkPhysicalDevice device);

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory);

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBufffer, VkDeviceSize size);

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkImage>& image, VDeleter<VkDeviceMemory>& imageMemory);

	void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	VkCommandBuffer beginSingleTimeCommands();

	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VDeleter<VkImageView>& imageView);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat findDepthFormat();

	bool hasStencilComponent(VkFormat format);

	static std::vector<char> readFile(const std::string& filename);
};

#endif // !VULKAN_RENDERER_H