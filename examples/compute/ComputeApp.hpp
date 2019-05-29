#ifndef COMPUTE_APP_HPP
#define COMPUTE_APP_HPP

#include <vulkan/vulkan.hpp>

#include <cstdio>
#include <cmath>

/** Callback */

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT,
	VkDebugUtilsMessageTypeFlagsEXT,
	const VkDebugUtilsMessengerCallbackDataEXT*,
	void*);


const int WIDTH = 1024;			// Size of rendered mandelbrot set.
const int HEIGHT = 1024;		// Size of renderered mandelbrot set.
const int WORKGROUP_SIZE = 16;	// Workgroup size in compute shader.

class ComputeApp {
public:
	ComputeApp();
	~ComputeApp();

	void run();
private:
	struct Pixel;

	vk::Instance instance;
	vk::DebugUtilsMessengerEXT callback;

	vk::PhysicalDevice physicalDevice;

	vk::Device device;
	vk::Queue computeQueue;

	vk::Pipeline pipeline;
	vk::PipelineLayout pipelineLayout;

	vk::CommandPool commandPool;
	vk::CommandBuffer commandBuffer;

	vk::DescriptorPool descriptorPool;
	vk::DescriptorSet descriptorSet;
	vk::DescriptorSetLayout descriptorSetLayout;

	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;

	vk::Buffer uniformBuffer;
	vk::DeviceMemory uniformBufferMemory;

	uint32_t bufferSize; // size of buffer in bytes

	void init();

	void updateUniformBuffer();
	void draw();

	void cleanup();

	void createInstance();
	void setupDebugCallback();
	void pickPhysicalDevice();
	void createDevice();
	void createBuffer();
	void createUniformBuffer();
	void createDescriptorSetLayout();
	void createDescriptorSet();
	void createComputePipeline();
	void createCommandeBuffer();

	struct Pixel {
		float r, g, b, a;
	};

	struct ComputeUBO {
		int width;
		int height;
	};
};

#endif
