#include "Render.hpp"

#include "fileUtils.hpp"

#include <ostream>
#include <set>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

Render::Render(std::string appName, uint32_t appVersion, RenderType type) : appName(appName), appVersion(appVersion), renderType(type), validationLayerEnabled(false) {

}

Render::~Render() {
	cleanup();
}

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
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createCommandPool();
	createTextureImage();
	createTextureImageView();
	createTextureSampler();
	createDepthResources();
	createFramebuffers();
	createBuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSemaphores();
	createFences();
}

void Render::cleanupSwapchain() {

	if (!commandBuffers.empty())
		device.freeCommandBuffers(commandPool, commandBuffers, deviceLoader);

	if (depthImageView)
		device.destroyImageView(depthImageView, nullptr, deviceLoader);
	if (depthImage)
		device.destroyImage(depthImage, nullptr, deviceLoader);
	if (depthImageMemory)
		device.free(depthImageMemory, nullptr, deviceLoader);

	if (graphicsPipeline)
		device.destroyPipeline(graphicsPipeline, nullptr, deviceLoader);
	if (pipelineLayout)
		device.destroyPipelineLayout(pipelineLayout, nullptr, deviceLoader);
	if (renderPass)
		device.destroyRenderPass(renderPass, nullptr, deviceLoader);

	for (size_t i = 0; i < swapchain.getImageCount(); i++) {
		device.destroyBuffer(uniformBuffers[i], nullptr, deviceLoader);
		device.free(uniformBuffersMemory[i], nullptr, deviceLoader);
		device.destroyEvent(uniformEvent[i], nullptr, deviceLoader);
	}

	if (descriptorPool)
		device.destroyDescriptorPool(descriptorPool, nullptr, deviceLoader);

}

void Render::cleanup() {

	device.waitIdle(deviceLoader);

	// Destroy device related objects
	cleanupSwapchain();
	swapchain.cleanup();

	device.destroySampler(textureSampler, nullptr, deviceLoader);

	for (int i = 0; i < textureImages.size(); i++) {
		device.destroyImageView(textureImageViews[i], nullptr, deviceLoader);
		device.destroyImage(textureImages[i], nullptr, deviceLoader);
		device.freeMemory(textureImagesMemory[i], nullptr, deviceLoader);
	}

	if (descriptorSetLayout)
		device.destroyDescriptorSetLayout(descriptorSetLayout, nullptr, deviceLoader);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (inFlightFences[i])
			device.destroyFence(inFlightFences[i], nullptr, deviceLoader);
		if (renderFinishedSemaphores[i])
			device.destroySemaphore(renderFinishedSemaphores[i], nullptr, deviceLoader);
		if (imageAvailableSemaphores[i])
			device.destroySemaphore(imageAvailableSemaphores[i], nullptr, deviceLoader);
	}

	for (size_t i = 0; i < buffers.size(); i++) {
		device.destroyBuffer(buffers[i], nullptr, deviceLoader);
		device.free(buffersMemory[i], nullptr, deviceLoader);
	}

	if (commandPool)
		device.destroyCommandPool(commandPool, nullptr, deviceLoader);

	// Destroy Device
	if (device)
		device.destroy(nullptr, deviceLoader);
	// Destroy instance related objects
	if (surface)
		instance.destroySurfaceKHR(surface, nullptr, instanceLoader);
	if (validationLayerEnabled) {
		instance.destroyDebugUtilsMessengerEXT(callback, nullptr, instanceLoader);
	}
	// Then destroy the instance
	if (instance)
		instance.destroy(nullptr, instanceLoader);
}

void Render::recreateSwapChain() {
	device.waitIdle(deviceLoader);

	cleanupSwapchain();

	swapchain.recreate(surface, physicalDevice, windowExtent);

	createRenderPass();
	createGraphicsPipeline();
	createDepthResources();
	createFramebuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
}

void Render::drawFrame() {
	device.waitForFences(inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max(), deviceLoader);

	uint32_t imageIndex;
	try {
		auto result = device.acquireNextImageKHR(swapchain.getSwapchain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], nullptr, deviceLoader);
		imageIndex = result.value;
	} catch (const vk::OutOfDateKHRError& e) {
		recreateSwapChain();
		return;
	}

	vk::SubmitInfo submitInfo;
	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	device.resetFences(inFlightFences[currentFrame], deviceLoader);

	graphicsQueue.submit(submitInfo, inFlightFences[currentFrame], deviceLoader);

	vk::PresentInfoKHR presentInfo;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	vk::SwapchainKHR swapChains[] = { swapchain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	try {
		auto result = presentQueue.presentKHR(presentInfo, deviceLoader);
		if (result == vk::Result::eSuboptimalKHR) {
			recreateSwapChain();
		}
	} catch (const vk::OutOfDateKHRError& e) {
		recreateSwapChain();
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	presentQueue.waitIdle(deviceLoader);
}

void Render::setRenderType(RenderType newType) {
	renderType = newType;
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

void Render::setSurfaceCreationFunction(createSurfaceFoncter functer) {
	surfaceCreation = functer;
}

void Render::setExtent(const vk::Extent2D& extent) {
	windowExtent = extent;
}

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

	instance = vk::createInstance(instanceInfo, nullptr, loader);
	loader.loadInstanceLevelEntryPoints(instance);
	instanceLoader.init(instance, loader.vkGetInstanceProcAddr);
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
	callback = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, instanceLoader);
}

void Render::pickPhysicalDevice() {
	auto devices = instance.enumeratePhysicalDevices(instanceLoader);

	if (devices.empty())
		throw std::runtime_error("Failed to find GPUs with Vulkan suport!");

	vk::QueueFlags queues;
	if ((renderType & RenderTypeBits::eGraphics) == RenderTypeBits::eGraphics)
		queues |= vk::QueueFlagBits::eGraphics;
	if ((renderType & RenderTypeBits::eCompute) == RenderTypeBits::eCompute)
		queues |= vk::QueueFlagBits::eCompute;

	for (const auto& device: devices) {
		if (isDeviceSuitable(device, deviceExtensions, queues, surface, instanceLoader)) {
			physicalDevice = device;
			break;
		}
	}

	if (!physicalDevice)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

void Render::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface, instanceLoader);

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

	device = physicalDevice.createDevice(createInfo, nullptr, instanceLoader);
	loader.loadDeviceLevelEntryPoints(device);
	deviceLoader.init(VkInstance(instance), loader.vkGetInstanceProcAddr, device, loader.vkGetDeviceProcAddr);

	if (renderType & RenderTypeBits::eGraphics)
		graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0, deviceLoader);

	if (renderType & RenderTypeBits::eCompute)
		computeQueue = device.getQueue(indices.computeFamily.value(), 0, deviceLoader);

	if (surface)
		presentQueue = device.getQueue(indices.presentFamily.value(), 0, deviceLoader);
}

void Render::createSurface() {
	if (surfaceCreation)
		surface = surfaceCreation(instance, instanceLoader);
}

void Render::createSwapchain(vk::SwapchainKHR oldSwapchain) {
	swapchain.init(surface, physicalDevice, device, windowExtent, oldSwapchain, deviceLoader);
}


void Render::createRenderPass() {
	vk::AttachmentDescription colorAttachment;
	colorAttachment.format = swapchain.getImageFormat();
	colorAttachment.samples = vk::SampleCountFlagBits::e1;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef;
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::AttachmentDescription depthAttachment;
	depthAttachment.format = findDepthFormat(physicalDevice, deviceLoader);
	depthAttachment.samples = vk::SampleCountFlagBits::e1;
	depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;	// What to do when writing final image
	depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
	depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::AttachmentReference depthAttachmentRef;
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	// index of colorAttachment is referenced in the fragment shader as layout(location = 0)
	subpass.pDepthStencilAttachment = &depthAttachmentRef;

	vk::SubpassDependency dependency;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.srcAccessMask = vk::AccessFlags();

	dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

	std::array<vk::AttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	renderPass = device.createRenderPass(renderPassInfo, nullptr, deviceLoader);
}

void Render::createDescriptorSetLayout() {
	std::vector<vk::DescriptorSetLayoutBinding> uboLayoutBindings;

	for (unsigned int i = 0; i < uniformStages.size(); i++) {
		auto stages = uniformStages[i];

		vk::DescriptorSetLayoutBinding uboLayoutBinding;
		uboLayoutBinding.binding = i;
		uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = stages;

		uboLayoutBindings.push_back(uboLayoutBinding);
	}

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.bindingCount = uboLayoutBindings.size();
	layoutInfo.pBindings = uboLayoutBindings.data();

	descriptorSetLayout = device.createDescriptorSetLayout(layoutInfo, nullptr, deviceLoader);

}

void Render::createGraphicsPipeline() {

	Shader vert(device, vk::ShaderStageFlagBits::eVertex, vertexShaderFile, deviceLoader);
	Shader frag(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderFile, deviceLoader);

	// Create the actuals shaders and link them
	vk::PipelineShaderStageCreateInfo vertShaderStageInfo = vert.getShaderStageInfo();

	vk::PipelineShaderStageCreateInfo fragShaderStageInfo = frag.getShaderStageInfo();

	vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	// Input of the vertex buffer
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Primitive style
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Viewport
	vk::Viewport viewport;
	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width = static_cast<float>(swapchain.getExtent().width);
	viewport.height = static_cast<float>(swapchain.getExtent().height);
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	// Scissor
	vk::Rect2D scissor;
	scissor.offset = vk::Offset2D(0, 0);
	scissor.extent = swapchain.getExtent();

	vk::PipelineViewportStateCreateInfo viewportState;
	// Using multiple viewports require GPU feature
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// Rasterization
	vk::PipelineRasterizationStateCreateInfo rasterizer;
	// If true, clamp fragments beyond near and far to them. Require GPU feature
	rasterizer.depthClampEnable = VK_FALSE; // can be set to True for shadowmap
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = vk::PolygonMode::eFill; // any other than fill require a GPU feature
	rasterizer.lineWidth = 1.0f; // require wideLines feature for lines thicker than 1 fragment
	// Culling
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
	// Depth bias (can be used for shadowmapping)
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.f;
	rasterizer.depthBiasClamp = 0.f;
	rasterizer.depthBiasSlopeFactor = 0.f;
	// Multisampling
	vk::PipelineMultisampleStateCreateInfo multisampling;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisampling.minSampleShading = 1.f;
	multisampling.pSampleMask = nullptr;
	multisampling.alphaToCoverageEnable = VK_FALSE;
	multisampling.alphaToOneEnable = VK_FALSE;

	// Depth test & Stencil test
	vk::PipelineDepthStencilStateCreateInfo depthStencil;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = vk::CompareOp::eLess;

	// Color blending
	// One PipelineColorBlendAttachmentState per attached framebuffer
	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask =
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
	colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
	colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
	colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;

	vk::PipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.logicOpEnable = VK_FALSE; // setting to TRUE automatically disable blendAttachment
	colorBlending.logicOp = vk::LogicOp::eCopy;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// Dynamic state
	// Allows to change some parameters without recreating the whole pipeline
	// See DynamicState to know the ones that can be dynamically set

	// Pipeline Layout
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo, nullptr, deviceLoader);

	// Graphics Pipeline
	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr;

	pipelineInfo.layout = pipelineLayout;

	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;

	graphicsPipeline = device.createGraphicsPipeline(nullptr, pipelineInfo, nullptr, deviceLoader);

}

void Render::createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface, deviceLoader);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	commandPool = device.createCommandPool(poolInfo, nullptr, deviceLoader);
}

void Render::createTextureImage() {
	for (unsigned int i = 0; i < imageFilenames.size(); i++) {
		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(imageFilenames[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		vk::DeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels)
			throw std::runtime_error("Failed to load texture image!");

		vk::Buffer stagingBuffer;
		vk::DeviceMemory stagingBufferMemory;
		createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

		::fillBuffer(device, stagingBufferMemory, pixels, imageSize, deviceLoader);

		stbi_image_free(pixels);

		textureImages.emplace_back();
		textureImagesMemory.emplace_back();

		createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImages[i], textureImagesMemory[i]);

		transitionImageLayout(textureImages[i], vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

		copyBufferToImage(stagingBuffer, textureImages[i], static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

		transitionImageLayout(textureImages[i], vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

		device.freeMemory(stagingBufferMemory, nullptr, deviceLoader);
		device.destroyBuffer(stagingBuffer, nullptr, deviceLoader);
	}
}

void Render::createTextureImageView() {
	for (unsigned i = 0; i < textureImages.size(); i++) {
		textureImageViews.push_back(createImageView(textureImages[i], vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor));
	}
}

void Render::createDepthResources() {
	vk::Format depthFormat = findDepthFormat(physicalDevice, deviceLoader);

	createImage(swapchain.getExtent().width, swapchain.getExtent().height, depthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);

	depthImageView = createImageView(depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
}

void Render::createFramebuffers() {
	swapchain.createFramebuffers(renderPass, depthImageView);
}

void Render::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& memory) {
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	buffer = device.createBuffer(bufferInfo, nullptr, deviceLoader);

	auto memRequirements = device.getBufferMemoryRequirements(buffer, deviceLoader);

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties, deviceLoader);

	memory = device.allocateMemory(allocInfo, nullptr, deviceLoader);

	device.bindBufferMemory(buffer, memory, /*memoryOffset*/ 0, deviceLoader);
}

void Render::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory) {
	vk::ImageCreateInfo imageInfo;
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = usage;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;

	image = device.createImage(imageInfo, nullptr, deviceLoader);

	vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements(image, deviceLoader);

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties, deviceLoader);

	imageMemory = device.allocateMemory(allocInfo, nullptr, deviceLoader);

	device.bindImageMemory(image, imageMemory, /*memoryOffset*/0, deviceLoader);
}

vk::ImageView Render::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspect) {
	vk::ImageViewCreateInfo createInfo;
	createInfo.image = image;
	createInfo.viewType = vk::ImageViewType::e2D;
	createInfo.format = format;
	// components swizzle
	createInfo.components.r = vk::ComponentSwizzle::eIdentity;
	createInfo.components.g = vk::ComponentSwizzle::eIdentity;
	createInfo.components.b = vk::ComponentSwizzle::eIdentity;
	//
	createInfo.subresourceRange.aspectMask = aspect;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	return device.createImageView(createInfo, nullptr, deviceLoader);
}

void Render::createTextureSampler() {
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = vk::CompareOp::eAlways;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	textureSampler = device.createSampler(samplerInfo, nullptr, deviceLoader);
}

void Render::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

		if (hasStencilComponent(format))
			barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
	}

	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
		//barrier.srcAccessMask = 0;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
		dstStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		srcStage = vk::PipelineStageFlagBits::eTransfer;
		dstStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		//barrier.srcAccessMask = 0;
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
		dstStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
	}
	else
		throw std::invalid_argument("Unsupported Layout transition !");

	commandBuffer.pipelineBarrier(srcStage, dstStage, vk::DependencyFlags(), nullptr, nullptr, barrier, deviceLoader);

	endSingleTimeCommands(commandBuffer);
}

void Render::createBuffers() {
	buffers.clear();
	buffersMemory.clear();
	for (unsigned int i = 0; i < bufferSizes.size(); i++) {
		buffers.push_back(vk::Buffer());
		buffersMemory.push_back(vk::DeviceMemory());
		createBuffer(bufferSizes[i], bufferUsages[i], vk::MemoryPropertyFlagBits::eDeviceLocal, buffers[i], buffersMemory[i]);
	}
}

void Render::createUniformBuffers() {
	unsigned int uniformCount = uniformSizes.size();
	unsigned int swapChainImageCount = swapchain.getImageCount();

	uniformBuffers.resize(uniformCount * swapChainImageCount);
	uniformBuffersMemory.resize(uniformCount * swapChainImageCount);

	for (unsigned int uniform = 0; uniform < uniformCount; uniform++)
		for (unsigned int image = 0; image < swapChainImageCount; image++) {
			unsigned int buffIndex = image + swapChainImageCount * uniform;
			createBuffer(uniformSizes[uniform], vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, uniformBuffers[buffIndex], uniformBuffersMemory[buffIndex]);
		}

	// Create fences to know which uniform to update
	uniformEvent.resize(swapChainImageCount);
	vk::EventCreateInfo createInfo;
	for (unsigned int image = 0; image < swapChainImageCount; image++) {
		uniformEvent[image] = device.createEvent(createInfo, nullptr, deviceLoader);
	}
}

void Render::createDescriptorPool() {
	uint32_t swapChainImageCount = static_cast<uint32_t>(swapchain.getImageCount());
	uint32_t uniformCount = static_cast<uint32_t>(uniformSizes.size());

	vk::DescriptorPoolSize poolSize;
	poolSize.type = vk::DescriptorType::eUniformBuffer;
	poolSize.descriptorCount = swapChainImageCount * uniformCount;

	vk::DescriptorPoolCreateInfo createInfo;
	createInfo.poolSizeCount = 1;
	createInfo.pPoolSizes = &poolSize;
	createInfo.maxSets = swapChainImageCount;

	descriptorPool = device.createDescriptorPool(createInfo, nullptr, deviceLoader);
}

void Render::createDescriptorSets() {
	unsigned int swapChainImageCount = swapchain.getImageCount();
	unsigned int uniformCount = uniformSizes.size();
	std::vector<vk::DescriptorSetLayout> layouts(swapChainImageCount, descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets = device.allocateDescriptorSets(allocInfo, std::allocator<vk::DescriptorSet>(), deviceLoader);

	for (unsigned int uniform = 0; uniform < uniformCount; uniform++)
		for (unsigned int image = 0; image < swapChainImageCount; image++) {
			unsigned int bufferIndex = image + uniform * swapChainImageCount;
			vk::DescriptorBufferInfo bufferInfo;
			bufferInfo.buffer = uniformBuffers[bufferIndex];
			bufferInfo.offset = 0;
			bufferInfo.range = uniformSizes[uniform];

			vk::WriteDescriptorSet descriptorWrite;
			descriptorWrite.dstSet = descriptorSets[image];
			descriptorWrite.dstBinding = uniform;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			device.updateDescriptorSets(descriptorWrite, nullptr, deviceLoader);
		}
}

void Render::copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size) {
	auto commandBuffer = beginSingleTimeCommands();

	vk::BufferCopy copyRegion;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	commandBuffer.copyBuffer(src, dst, copyRegion, deviceLoader);

	endSingleTimeCommands(commandBuffer);
}

void Render::copyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height) {
	auto commandBuffer = beginSingleTimeCommands();

	vk::BufferImageCopy region;
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = vk::Offset3D(0, 0, 0);
	region.imageExtent = vk::Extent3D(width, height, 1);

	commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region, deviceLoader);

	endSingleTimeCommands(commandBuffer);
}

vk::CommandBuffer Render::beginSingleTimeCommands() {
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;

	auto commandBuffer = device.allocateCommandBuffers(allocInfo, deviceLoader)[0];

	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
	beginInfo.pInheritanceInfo = nullptr;

	commandBuffer.begin(beginInfo, deviceLoader);

	return commandBuffer;
}

void Render::endSingleTimeCommands(vk::CommandBuffer commandBuffer) {
	commandBuffer.end(deviceLoader);

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	graphicsQueue.submit(submitInfo, nullptr, deviceLoader);
	graphicsQueue.waitIdle(deviceLoader);

	device.freeCommandBuffers(commandPool, commandBuffer, deviceLoader);
}

void Render::createCommandBuffers() {
	commandBuffers.resize(swapchain.getImageCount());

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	commandBuffers = device.allocateCommandBuffers(allocInfo, std::allocator<vk::CommandBuffer>(), deviceLoader);

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		vk::CommandBufferBeginInfo beginInfo;
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
		beginInfo.pInheritanceInfo = nullptr;

		commandBuffers[i].begin(beginInfo, deviceLoader);

		vk::RenderPassBeginInfo renderPassInfo;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapchain.getFramebuffer(i);
		renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
		renderPassInfo.renderArea.extent = swapchain.getExtent();
		std::array<vk::ClearValue, 2> clearColors;
		clearColors[0].color = vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.f}));
		clearColors[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
		renderPassInfo.clearValueCount = clearColors.size();
		renderPassInfo.pClearValues = clearColors.data();

		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline, deviceLoader);

		commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline, deviceLoader);

		// TODO enhance to handle multiple meshes
		vk::Buffer vertexBuffers[1];
		vk::Buffer indexBuffer;
		uint32_t indexCount;

		for (unsigned int j = 0; j < bufferUsages.size(); j++) {
			if ((bufferUsages[j] & vk::BufferUsageFlagBits::eVertexBuffer) == vk::BufferUsageFlagBits::eVertexBuffer)
				vertexBuffers[0] = buffers[j];
			if ((bufferUsages[j] & vk::BufferUsageFlagBits::eIndexBuffer) == vk::BufferUsageFlagBits::eIndexBuffer) {
				indexBuffer = buffers[j];
				indexCount = bufferSizes[j] / sizeof(uint16_t);
			}
		}

		vk::DeviceSize offsets[1] = {0};

		commandBuffers[i].bindVertexBuffers(/*first*/0, /*count*/1, vertexBuffers, offsets, deviceLoader);
		commandBuffers[i].bindIndexBuffer(indexBuffer, /*offset*/0, vk::IndexType::eUint16, deviceLoader);

		commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, /*first set*/0, descriptorSets[i], nullptr, deviceLoader);

		commandBuffers[i].drawIndexed(indexCount, /*instance count*/1, /*first index*/0, /*vertex offset*/0, /*first instance*/0, deviceLoader);

		commandBuffers[i].endRenderPass(deviceLoader);

		commandBuffers[i].setEvent(uniformEvent[i], vk::PipelineStageFlagBits::eVertexShader, deviceLoader);

		commandBuffers[i].end(deviceLoader);
	}
}

void Render::createSemaphores() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

	vk::SemaphoreCreateInfo createInfo;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		imageAvailableSemaphores[i] = device.createSemaphore(createInfo, nullptr, deviceLoader);
		renderFinishedSemaphores[i] = device.createSemaphore(createInfo, nullptr, deviceLoader);
	}
}

void Render::createFences() {
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	vk::FenceCreateInfo createInfo;
	createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		inFlightFences[i] = device.createFence(createInfo, nullptr, deviceLoader);
}
