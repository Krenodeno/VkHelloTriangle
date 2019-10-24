#include "Render.hpp"

#include <iostream>

Render::Render(const std::string& appName, uint32_t appVersion, RenderType type) : appName(appName), appVersion(appVersion), renderType(type), validationLayerEnabled(false) {

}

Render::~Render() {
	cleanup();
}

void Render::setAppName(const std::string& name) {
	appName = name;
}

void Render::setAppVersion(uint32_t version) {
	appVersion = version;
}

void Render::setExtent(const vk::Extent2D& extent) {
	windowExtent = extent;
}

void Render::setRenderType(RenderType newType) {
	renderType = newType;
}

void Render::setSurfaceCreationFunction(createSurfaceFoncter fun) {
	surfaceCreation = fun;
}

void Render::addLayer(const char* layerName) {
	layers.push_back(layerName);
}

void Render::addInstanceExtension(const char* extensionName) {
	instanceExtensions.push_back(extensionName);
}

void Render::addDeviceExtension(const char* extensionName) {
	deviceExtensions.push_back(extensionName);
}

void Render::enableValidationLayer() {
	if (validationLayerEnabled == false) {
		validationLayerEnabled = true;
		layers.push_back("VK_LAYER_KHRONOS_validation");
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}


void Render::waitDeviceIdle() {
	device->waitIdle(deviceLoader);
}

void Render::waitForFences() {
	//device->waitForFences(fences, VK_FALSE, std::numeric_limits<uint64_t>::max(), deviceLoader);
}
/*
unsigned int Render::addBuffer(uint64_t size, vk::BufferUsageFlags usage, bool writeFromHost, bool readFromHost) {
	bufferSizes.push_back(size);
	if (writeFromHost)
		usage |= vk::BufferUsageFlagBits::eTransferDst;
	if (readFromHost)
		usage |= vk::BufferUsageFlagBits::eTransferSrc;
	bufferUsages.push_back(usage);
	return (int)bufferSizes.size() - 1;
}

void Render::fillBuffer(unsigned int bufferIndex, const void* data, uint64_t dataSize) {
	// Create staging buffer
	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	createBuffer(dataSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);
	// Copy data from CPU memory to staging buffer memory
	::fillBuffer(device, stagingBufferMemory, data, dataSize, deviceLoader);
	// Copy data from staging buffer memory to GPU memory
	copyBuffer(stagingBuffer, buffers[bufferIndex], dataSize);
	// Destroy staging buffer
	device.freeMemory(stagingBufferMemory, nullptr, deviceLoader);
	device.destroyBuffer(stagingBuffer, nullptr, deviceLoader);
}
*/
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	vk::DebugUtilsMessageTypeFlagsEXT mType(messageType);

	std::ostream& out = [&]() -> std::ostream& {
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			return (std::cerr);
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			return (std::cerr);
		return (std::clog);
	}();

	out << vk::to_string(mType) << " ";

	out << pCallbackData->pMessage << "\n";

	return VK_FALSE;
}

void Render::init() {
	createInstance();
	setupDebugCallback();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapchain();
	/*
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createTextureImages();
	createTextureImageViews();
	createTextureSampler(16f, static_cast<float>(textures[0].mipLevels));
	createDepthResources();
	createFramebuffers();
	createBuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSemaphores();
	createFences();
	*/
}

void Render::cleanup() {

}

void drawFrame() {

}

/*****************************************************************************/
/***                            INITIALISATION                             ***/
/*****************************************************************************/

/**
 * Initialisation of a Vulkan application: Instance Creation
 */
void Render::createInstance() {

	std::cout << "Instance creation\n";
	std::cout << "  Required layers: \n";
	for (auto layer : layers) {
		std::cout << "\t" << layer << "\t";
		if (!checkLayerSupport(layer, loader)) {
			std::string error_msg1(layer);
			std::string error_msg2(" not supported !");
			throw std::runtime_error(error_msg1 + error_msg2);
		}
		std::cout << "\n";
	}

	std::cout << "  Required extensions: \n";
	for (auto extension : instanceExtensions) {
		std::cout << "\t" << extension << "\t";
		if (checkExtensionSupport(extension, loader))
			std::cout << "supported\n";
		else {
			std::cerr << "non supported\n";
		}
	}

	// Remove non-supported extensions
	instanceExtensions.erase(
		std::remove_if(instanceExtensions.begin(), instanceExtensions.end(), [&](const char* e){ return !checkExtensionSupport(e, loader); }),
		instanceExtensions.end());

	// Check if there is minimum 2 Surface extensions (VK_KHR_surface and platform Surface)
	if (std::count_if(instanceExtensions.begin(), instanceExtensions.end(), [](std::string s){return s.find("surface") != std::string::npos;}) < 2)
		throw std::runtime_error("No supported surface extenstions");

	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = appName.c_str();
	appInfo.applicationVersion = appVersion;
	appInfo.pEngineName = "HelloTriangle";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = layers.size();
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledExtensionCount = instanceExtensions.size();
	instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();

	instance = vk::createInstanceUnique(instanceInfo, nullptr, loader);
	loader.loadInstanceLevelEntryPoints(instance.get());
	instanceLoader.init(instance.get(), loader.vkGetInstanceProcAddr);
}

void Render::setupDebugCallback() {
	if (!validationLayerEnabled) return;

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
	messenger = instance->createDebugUtilsMessengerEXTUnique(createInfo, nullptr, instanceLoader);
}

void Render::pickPhysicalDevice() {
	auto devices = instance->enumeratePhysicalDevices(instanceLoader);

	if (devices.empty())
		throw std::runtime_error("Failed to find GPUs with Vulkan suport!");

	vk::QueueFlags queues;
	if ((renderType & RenderTypeBits::eGraphics) == RenderTypeBits::eGraphics)
		queues |= vk::QueueFlagBits::eGraphics;
	if ((renderType & RenderTypeBits::eCompute) == RenderTypeBits::eCompute)
		queues |= vk::QueueFlagBits::eCompute;

	for (const auto& device: devices) {
		if (isDeviceSuitable(device, deviceExtensions, queues, surface.get(), instanceLoader)) {
			physicalDevice = device;
			break;
		}
	}

	if (!physicalDevice)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

void Render::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface.get(), instanceLoader);

	// Queues infos
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<unsigned int> uniqueFamilies;
	if (surface)
		uniqueFamilies.insert(indices.presentFamily.value());
	if (renderType & RenderTypeBits::eGraphics)
		uniqueFamilies.insert(indices.graphicsFamily.value());
	if (renderType & RenderTypeBits::eCompute)
		uniqueFamilies.insert(indices.computeFamily.value());

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueFamilies) {
		vk::DeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	// Features
	vk::PhysicalDeviceFeatures deviceFeatures;
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	// Device creation
	vk::DeviceCreateInfo createInfo;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// For backward compatibility only (device layers have been deprecated)
	if (validationLayerEnabled) {
		createInfo.enabledLayerCount = layers.size();
		createInfo.ppEnabledLayerNames = layers.data();
	}
	else
		createInfo.enabledLayerCount = 0;

	device = physicalDevice.createDeviceUnique(createInfo, nullptr, instanceLoader);
	loader.loadDeviceLevelEntryPoints(device.get());
	deviceLoader.init(VkInstance(instance.get()), loader.vkGetInstanceProcAddr, device.get(), loader.vkGetDeviceProcAddr);

	if (renderType & RenderTypeBits::eGraphics)
		graphicQueue = device->getQueue(indices.graphicsFamily.value(), 0, deviceLoader);

	if (renderType & RenderTypeBits::eCompute)
		computeQueue = device->getQueue(indices.computeFamily.value(), 0, deviceLoader);

	if (surface)
		presentQueue = device->getQueue(indices.presentFamily.value(), 0, deviceLoader);
}

void Render::createSurface() {
	if (surfaceCreation) {
		auto deleter = vk::ObjectDestroy<vk::Instance, vk::DispatchLoaderDynamic>(instance.get(), nullptr, instanceLoader);
		surface = vk::UniqueHandle<vk::SurfaceKHR, vk::DispatchLoaderDynamic>(surfaceCreation(instance.get(), instanceLoader), deleter);
	}
}

void Render::createSwapchain(vk::SwapchainKHR oldSwapchain) {
	swapchain.init(surface.get(), physicalDevice, device.get(), windowExtent, oldSwapchain, deviceLoader);
}

void Render::createFramebuffers(vk::RenderPass renderPass, vk::ImageView depthImageView) {
	uint32_t imageCount = swapchain.getImageCount();

	for(size_t i = 0; i < swapChainImageViews.size(); ++i) {
		std::vector<vk::ImageView> attachments;
		attachments.push_back(swapChainImageViews[i]);
		if (depthImageView)
			attachments.push_back(depthImageView);
		// (From vulkan-tutorial) Same depth image can be used because only a single subpass is running at the same time due to our semaphores
		// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;

		swapChainFramebuffers[i] = device.createFramebuffer(framebufferInfo, nullptr, deviceLoader);
	}
}