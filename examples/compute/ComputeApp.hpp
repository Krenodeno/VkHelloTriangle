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

static std::vector<uint32_t> readFile(uint32_t& length, std::string filename) {
	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == NULL) {
		throw std::runtime_error("Could not find or open file\n");
	}

	// get file size.
	fseek(fp, 0, SEEK_END);
	long filesize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	long filesizepadded = long(ceil(filesize / 4.0)) * 4;

	// read file contents.
	std::vector<uint32_t> str(filesizepadded);
	fread(str.data(), filesize, sizeof(char), fp);
	fclose(fp);

	// data padding.
	for (int i = filesize; i < filesizepadded; i++) {
		str[i] = 0;
	}

	length = filesizepadded;
	return str;
}

const int WIDTH = 1024; // Size of rendered mandelbrot set.
const int HEIGHT = 1024; // Size of renderered mandelbrot set.
const int WORKGROUP_SIZE = 16; // Workgroup size in compute shader.

class ComputeApp {
public:
	ComputeApp();
	~ComputeApp();

	void run();
private:

	struct QueueFamilyIndices;
	struct Pixel;

	vk::Instance instance;
	vk::DebugUtilsMessengerEXT callback;

	vk::PhysicalDevice physicalDevice;

	vk::Device device;
	vk::Queue computeQueue;

	vk::Pipeline pipeline;
	vk::PipelineLayout pipelineLayout;
	vk::ShaderModule computeShaderModule;

	vk::CommandPool commandPool;
	vk::CommandBuffer commandBuffer;

	vk::DescriptorPool descriptorPool;
	vk::DescriptorSet descriptorSet;
	vk::DescriptorSetLayout descriptorSetLayout;

	vk::Buffer buffer;
	vk::DeviceMemory bufferMemory;

	uint32_t bufferSize; // size of buffer in bytes

	void init();

	void draw();

	void cleanup();

	void createInstance();
	void setupDebugCallback();
	void pickPhysicalDevice();
	void createDevice();
	void allocateMemory();
	void createBuffer();
	void createDescriptorSetLayout();
	void createDescriptorSet();
	void createComputePipeline();
	void createCommandeBuffer();

	bool isDeviceSuitable(vk::PhysicalDevice);

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice);

	uint32_t findMemoryType(uint32_t, vk::MemoryPropertyFlags);

	struct QueueFamilyIndices {
		int computeFamily = -1;

		bool isComplete() {
			return computeFamily >= 0;
		}
	};

	struct Pixel {
		float r, g, b, a;
	};
};

#endif
