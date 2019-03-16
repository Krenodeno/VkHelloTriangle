#include "ComputeApp.hpp"

#include "RenderUtils.hpp"
#include "Shader.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	//if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	std::ostream& out = [&]() -> std::ostream& {
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			return (std::cout);
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			return (std::cerr);
		return (std::clog);
	}();

	if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
		out << "[GENERAL]     ";
	if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
		out << "[PERFORMANCE] ";
	if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		out << "[VALIDATION]  ";

	out << pCallbackData->pMessage << "\n";

	return VK_FALSE;
}

ComputeApp::ComputeApp() : bufferSize(sizeof(Pixel) * WIDTH * HEIGHT) {

}

ComputeApp::~ComputeApp() {
	cleanup();
}

void ComputeApp::run() {
	init();
	updateUniformBuffer();
	draw();

	// Get data back
	void* mappedMemory;
	mappedMemory = device.mapMemory(bufferMemory, /*offset*/ 0, bufferSize);

	assert(mappedMemory);

	Pixel* pMappedMemory = static_cast<Pixel*>(mappedMemory);

	std::vector<unsigned char> image;
	image.reserve(WIDTH * HEIGHT * 4);
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		image.push_back(static_cast<unsigned char>(255.0f * pMappedMemory[i].r));
		image.push_back(static_cast<unsigned char>(255.0f * pMappedMemory[i].g));
		image.push_back(static_cast<unsigned char>(255.0f * pMappedMemory[i].b));
		//image.push_back(static_cast<unsigned char>(255.0f * pMappedMemory[i].a));
	}

	device.unmapMemory(bufferMemory);
	// Save image
	std::ofstream file;
	file.open("image.ppm");

	if (!file.is_open())
		throw std::runtime_error("Cannot open file for saving image");

	file << "P3\n" << WIDTH << " " << HEIGHT << "\n" << 255 << "\n";
	for (int i = 0; i < image.size(); i++) {
		file << int(image[i]) << " ";
		if (i % WIDTH == 0 && i != 0)
			file << "\n";
	}

	file.flush();
	file.close();
}

void ComputeApp::updateUniformBuffer() {
	ComputeUBO ubo = {WIDTH, HEIGHT};

	// Copy data in ubo
	auto data = static_cast<ComputeUBO*>(device.mapMemory(uniformBufferMemory, /*offset*/ 0, sizeof(ubo)));
		std::memcpy(data, &ubo, sizeof(ubo));
	device.unmapMemory(uniformBufferMemory);
}

void ComputeApp::draw() {
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vk::Fence fence;
	vk::FenceCreateInfo fenceCreateInfo;
	fence = device.createFence(fenceCreateInfo);

	computeQueue.submit(submitInfo, fence);

	device.waitForFences(fence, VK_TRUE, std::numeric_limits<uint64_t>::max());

	device.destroyFence(fence);
}

void ComputeApp::init() {
	createInstance();
	setupDebugCallback();
	pickPhysicalDevice();
	createDevice();
	createBuffer();
	createUniformBuffer();
	createDescriptorSetLayout();
	createDescriptorSet();
	createComputePipeline();
	createCommandeBuffer();
}

void ComputeApp::cleanup() {
	// Destroy vulkan objects
	if (commandPool) device.destroy(commandPool);
	if (pipeline) device.destroy(pipeline);
	if (pipelineLayout) device.destroy(pipelineLayout);
	if (descriptorPool) device.destroy(descriptorPool);
	if (descriptorSetLayout) device.destroy(descriptorSetLayout);
	if (uniformBufferMemory) device.free(uniformBufferMemory);
	if (uniformBuffer) device.destroy(uniformBuffer);
	if (bufferMemory) device.free(bufferMemory);
	if (buffer) device.destroy(buffer);
	// Then destroy Device
	if (device) device.destroy();
	// destroy debug utils
	auto func = (PFN_vkGetInstanceProcAddr)instance.getProcAddr("vkGetInstanceProcAddr");
	if (callback) instance.destroyDebugUtilsMessengerEXT(callback, nullptr, vk::DispatchLoaderDynamic(instance, func));
	// Finally destroy the instance
	if (instance) instance.destroy();
}

void ComputeApp::createInstance() {
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = "Compute Application";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "NOENGINE";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	std::vector<const char*> extensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
	std::vector<const char*> layers = { "VK_LAYER_LUNARG_standard_validation" };

	vk::InstanceCreateInfo createInfo;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = layers.size();
	createInfo.ppEnabledLayerNames = layers.data();

	instance = vk::createInstance(createInfo);
}

void ComputeApp::setupDebugCallback() {
	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	createInfo.messageSeverity =
		//vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		//vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
	createInfo.messageType =
		//vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;

	auto func = (PFN_vkGetInstanceProcAddr)instance.getProcAddr("vkGetInstanceProcAddr");
	vk::DispatchLoaderDynamic didy(instance, func);
	callback = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, didy);
}

void ComputeApp::pickPhysicalDevice() {

	auto devices = instance.enumeratePhysicalDevices();

	if (devices.empty())
		throw std::runtime_error("Failed to find GPUs with Vulkan suport!");

	for (const auto& device: devices) {
		if (isDeviceSuitable(device, {}, vk::QueueFlagBits::eCompute)) {
			physicalDevice = device;
			break;
		}
	}

	if (!physicalDevice)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

void ComputeApp::createDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

	// Queues
	float priority = 1.f;
	vk::DeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.queueFamilyIndex = indices.computeFamily.value();
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &priority;


	vk::PhysicalDeviceFeatures physicalDeviceFeatures;

	std::vector<const char*> extensions = {};

	vk::DeviceCreateInfo createInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.pEnabledFeatures = &physicalDeviceFeatures;
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 0;

	device = physicalDevice.createDevice(createInfo);

	// get Queue
	computeQueue = device.getQueue(indices.computeFamily.value(), 0);

}

void ComputeApp::createBuffer() {
	vk::BufferCreateInfo createInfo;
	createInfo.size = bufferSize;
	createInfo.usage = vk::BufferUsageFlagBits::eStorageBuffer;
	createInfo.sharingMode = vk::SharingMode::eExclusive;

	buffer = device.createBuffer(createInfo);

	auto memoryRequirements = device.getBufferMemoryRequirements(buffer);

	vk::MemoryAllocateInfo allocateInfo;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

	bufferMemory = device.allocateMemory(allocateInfo);

	device.bindBufferMemory(buffer, bufferMemory, /*MemoryOffset*/ 0);
}

void ComputeApp::createUniformBuffer() {
	vk::BufferCreateInfo createInfo;
	createInfo.size = sizeof(ComputeUBO);
	createInfo.usage = vk::BufferUsageFlagBits::eUniformBuffer;
	createInfo.sharingMode = vk::SharingMode::eExclusive;

	uniformBuffer = device.createBuffer(createInfo);

	vk::MemoryRequirements memoryRequirements = device.getBufferMemoryRequirements(uniformBuffer);

	vk::MemoryAllocateInfo allocateInfo;
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible);

	uniformBufferMemory = device.allocateMemory(allocateInfo);

	device.bindBufferMemory(uniformBuffer, uniformBufferMemory, /*memoryOffset*/ 0);
}

void ComputeApp::createDescriptorSetLayout() {
	vk::DescriptorSetLayoutBinding binding;
	binding.binding = 0;
	binding.descriptorType = vk::DescriptorType::eStorageBuffer;
	binding.descriptorCount = 1;
	binding.stageFlags = vk::ShaderStageFlagBits::eCompute;

	vk::DescriptorSetLayoutBinding uniformBinding;
	uniformBinding.binding = 1;
	uniformBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	uniformBinding.descriptorCount = 1;
	uniformBinding.stageFlags = vk::ShaderStageFlagBits::eCompute;

	vk::DescriptorSetLayoutBinding bindings[] = {binding, uniformBinding};

	vk::DescriptorSetLayoutCreateInfo createInfo;
	createInfo.bindingCount = 2;
	createInfo.pBindings = bindings;

	descriptorSetLayout = device.createDescriptorSetLayout(createInfo);
}

void ComputeApp::createDescriptorSet() {
	vk::DescriptorPoolSize poolSizes[2];
	poolSizes[0].type = vk::DescriptorType::eStorageBuffer;
	poolSizes[0].descriptorCount = 1;

	poolSizes[1].type = vk::DescriptorType::eUniformBuffer;
	poolSizes[1].descriptorCount = 1;

	vk::DescriptorPoolCreateInfo createInfo;
	createInfo.maxSets = 1;	// We only need to allocate one descriptor set from the pool
	createInfo.poolSizeCount = 2;
	createInfo.pPoolSizes = poolSizes;

	descriptorPool = device.createDescriptorPool(createInfo);

	// Allocate the set in the pool

	vk::DescriptorSetAllocateInfo allocateInfo;
	allocateInfo.descriptorPool = descriptorPool;
	allocateInfo.descriptorSetCount = 1;	// Allocate 1 descriptor set;
	allocateInfo.pSetLayouts = &descriptorSetLayout;

	descriptorSet = device.allocateDescriptorSets(allocateInfo)[0];

	// Connect buffer with descriptor
	vk::DescriptorBufferInfo bufferInfo;
	bufferInfo.buffer = buffer;
	bufferInfo.offset = 0;
	bufferInfo.range = bufferSize;

	vk::WriteDescriptorSet writeDescriptorSet;
	writeDescriptorSet.dstSet = descriptorSet;	// Write to this descriptor set
	writeDescriptorSet.dstBinding = 0;			// Write to the first binding
	writeDescriptorSet.descriptorCount = 1;
	writeDescriptorSet.descriptorType = vk::DescriptorType::eStorageBuffer;
	writeDescriptorSet.pBufferInfo = &bufferInfo;

	device.updateDescriptorSets(writeDescriptorSet, nullptr);

	vk::DescriptorBufferInfo uniformBufferInfo;
	uniformBufferInfo.buffer = uniformBuffer;
	uniformBufferInfo.offset = 0;
	uniformBufferInfo.range = sizeof(ComputeUBO);

	vk::WriteDescriptorSet uniformWriteDescriptorSet;
	uniformWriteDescriptorSet.dstSet = descriptorSet;
	uniformWriteDescriptorSet.dstBinding = 1;
	uniformWriteDescriptorSet.dstArrayElement = 0;
	uniformWriteDescriptorSet.descriptorType = vk::DescriptorType::eUniformBuffer;
	uniformWriteDescriptorSet.descriptorCount = 1;
	uniformWriteDescriptorSet.pBufferInfo = &uniformBufferInfo;

	device.updateDescriptorSets(uniformWriteDescriptorSet, nullptr);
}

void ComputeApp::createComputePipeline() {

	// Create Shader
	Shader computeShader(device, vk::ShaderStageFlagBits::eCompute);
	computeShader.create("ressources/shaders/shader.comp.spv");

	// Specify compute shader stage
	auto shaderStageCreateInfo = computeShader.getShaderStageInfo();

	// Create PipelineLayout
	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
	pipelineLayoutCreateInfo.setLayoutCount = 1;
	pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout;

	pipelineLayout = device.createPipelineLayout(pipelineLayoutCreateInfo);

	// Create Pipeline
	vk::ComputePipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.stage = shaderStageCreateInfo;
	pipelineCreateInfo.layout = pipelineLayout;

	pipeline = device.createComputePipeline(nullptr, pipelineCreateInfo);
}

void ComputeApp::createCommandeBuffer() {
	auto queueFamilyIndex = findQueueFamilies(physicalDevice);


	// Create Command Pool
	vk::CommandPoolCreateInfo poolCreateInfo;
	poolCreateInfo.queueFamilyIndex = queueFamilyIndex.computeFamily.value();
	commandPool = device.createCommandPool(poolCreateInfo);

	// Create Command buffer
	vk::CommandBufferAllocateInfo allocateInfo;
	allocateInfo.level = vk::CommandBufferLevel::ePrimary;
	allocateInfo.commandPool = commandPool;
	allocateInfo.commandBufferCount = 1;
	commandBuffer = device.allocateCommandBuffers(allocateInfo)[0];

	// Record some commands
	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	commandBuffer.begin(beginInfo);

	commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, pipeline);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, pipelineLayout, /*first set*/0, descriptorSet, nullptr);

	uint32_t dx = static_cast<uint32_t>(ceil(WIDTH / WORKGROUP_SIZE));
	uint32_t dy = static_cast<uint32_t>(ceil(HEIGHT / WORKGROUP_SIZE));

	std::cout << "Dispatch x = " << dx << ", y = " << dy << "\n";

	// run the compute shader
	commandBuffer.dispatch(dx, dy, 1);

	commandBuffer.end();
}
