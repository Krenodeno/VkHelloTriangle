#include "Render.hpp"

#include "Shader.hpp"

#include <iostream>
#include <chrono>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Render::Render(const std::string& appName, uint32_t appVersion, RenderType type)
	: appName(appName), appVersion(appVersion), renderType(type), validationLayerEnabled(false),
	instanceLoader(loader.vkGetInstanceProcAddr) {

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
*/

void Render::fillBuffer(Buffer buffer, const void* data, uint64_t dataSize) {
	// Create staging buffer
	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	createBuffer(dataSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);
	// Copy data from CPU memory to staging buffer memory
	::fillBuffer(device.get(), stagingBufferMemory, data, dataSize, deviceLoader);
	// Copy data from staging buffer memory to GPU memory
	copyBuffer(stagingBuffer, buffer.buffer, dataSize);
	// Destroy staging buffer
	device->freeMemory(stagingBufferMemory, nullptr, deviceLoader);
	device->destroyBuffer(stagingBuffer, nullptr, deviceLoader);
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
	createTexture();
	createTextureView();
	createTextureSampler(16.f, texture.mipLevels);
	//createDepthResources();
	createFramebuffers();
	createBuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSemaphores();
	createFences();
	
}

void Render::cleanup() {
	cleanupSwapchain();

	vertexBuffer.cleanup(device.get(), deviceLoader);
	indexBuffer.cleanup(device.get(), deviceLoader);
}

void Render::cleanupSwapchain() {
	pipelines[0].cleanup(device.get(), deviceLoader);
	pipelines[1].cleanup(device.get(), deviceLoader);

	for (unsigned int i = 0; i < swapchain.getImageCount(); i++) {
		sunMVPUniformBuffers[i].cleanup(device.get(), deviceLoader);
		viewMVPUniformBuffers[i].cleanup(device.get(), deviceLoader);
		lightUniformBuffers[i].cleanup(device.get(), deviceLoader);
	}

}

void Render::drawFrame() {
	device->waitForFences(inFlightFences[currentFrame].get(), VK_TRUE, std::numeric_limits<uint64_t>::max(), deviceLoader);

	uint32_t imageIndex;
	try {
		auto result = device->acquireNextImageKHR(swapchain.getSwapchain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame].get(), nullptr, deviceLoader);
		imageIndex = result.value;
	} catch (const vk::OutOfDateKHRError& e) {
		recreateSwapchain();
		return;
	}

	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	auto model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	auto lightPos = glm::vec3(2.0f, -2.0f, 2.0f);
	auto cameraPos = glm::vec3(2.0f, 2.0f, 2.0f);

	auto sunRotation = glm::rotate(glm::mat4(1.0f), time * glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	MVPUniformBufferObject sun = {};
		sun.model = model;
		sun.view = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		sun.proj = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 5.f);
		sun.proj[1][1] *= -1;

	MVPUniformBufferObject ubo = {};
		ubo.model = model;
		ubo.view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), swapchain.getExtent().width / (float)swapchain.getExtent().height, 0.1f, 5.0f);
		ubo.proj[1][1] *= -1;	// because of OpenGL upside down screen coordinates

	LightUniformBufferObject light = {};
		light.light = sun.proj * sun.view;
		light.lightWorldPos = glm::vec4(lightPos, 1.f);
		light.viewWorldPos = glm::vec4(cameraPos, 1.f);

	// Update Uniforms
	::fillBuffer(device.get(), viewMVPUniformBuffers[imageIndex].memory, &ubo, viewMVPUniformBuffers[imageIndex].size, deviceLoader);
	::fillBuffer(device.get(), sunMVPUniformBuffers[imageIndex].memory, &sun, sunMVPUniformBuffers[imageIndex].size, deviceLoader);
	::fillBuffer(device.get(), lightUniformBuffers[imageIndex].memory, &light, lightUniformBuffers[imageIndex].size, deviceLoader);

	vk::SubmitInfo submitInfo;
	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame].get() };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex].get();

	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	device->resetFences(inFlightFences[currentFrame].get(), deviceLoader);

	graphicQueue.submit(submitInfo, inFlightFences[currentFrame].get(), deviceLoader);

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
			recreateSwapchain();
		}
	} catch (const vk::OutOfDateKHRError& e) {
		recreateSwapchain();
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	presentQueue.waitIdle(deviceLoader);
}

void Render::fillTexture(void* pixels) {
	vk::DeviceSize imageSize = texture.width * texture.height * 4;

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	createBuffer(imageSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

	::fillBuffer(device.get(), stagingBufferMemory, pixels, imageSize, deviceLoader);

	transitionImageLayout(texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, texture.mipLevels);

	copyBufferToImage(stagingBuffer, texture.image, texture.width, texture.height);

	// Transitionned to ShaderOnlyOptimal while generating the mipmaps
	generateMipmaps(texture.image, vk::Format::eR8G8B8A8Unorm, texture.width, texture.height, texture.mipLevels);

	device->freeMemory(stagingBufferMemory, nullptr, deviceLoader);
	device->destroyBuffer(stagingBuffer, nullptr, deviceLoader);
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
		if (!checkLayerSupport(layer, instanceLoader)) {
			std::string error_msg1(layer);
			std::string error_msg2(" not supported !");
			throw std::runtime_error(error_msg1 + error_msg2);
		}
		std::cout << "\n";
	}

	std::cout << "  Required extensions: \n";
	for (auto extension : instanceExtensions) {
		std::cout << "\t" << extension << "\t";
		if (checkExtensionSupport(extension, instanceLoader))
			std::cout << "supported\n";
		else {
			std::cerr << "non supported\n";
		}
	}

	// Remove non-supported extensions
	instanceExtensions.erase(
		std::remove_if(instanceExtensions.begin(), instanceExtensions.end(), [&](const char* e){ return !checkExtensionSupport(e, instanceLoader); }),
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

	// TODO: SDK 1.1.126 include a loader to dynamically load vkGetInstanceProcAddr and all the other symbols
	instance = vk::createInstanceUnique(instanceInfo, nullptr, instanceLoader);
	//loader.loadInstanceLevelEntryPoints(instance.get());
	//instanceLoader.init(instance.get(), loader.vkGetInstanceProcAddr);
	instanceLoader.init(instance.get());
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
	deviceFeatures.depthClamp = VK_TRUE;

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

void Render::recreateSwapchain()
{
	device->waitIdle(deviceLoader);

	cleanupSwapchain();

	swapchain.recreate(surface.get(), physicalDevice, windowExtent);
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createFramebuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
}

/**
 * Pipeline creation
 */

void Render::createRenderPass() {

	// DEPTH PIPELINE
	vk::AttachmentDescription shadowDepthAttachment;
	shadowDepthAttachment.format = findDepthFormat(physicalDevice, deviceLoader);
	shadowDepthAttachment.samples = vk::SampleCountFlagBits::e1;
	shadowDepthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	shadowDepthAttachment.storeOp = vk::AttachmentStoreOp::eStore;	// What to do when writing final image
	shadowDepthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	shadowDepthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	shadowDepthAttachment.initialLayout = vk::ImageLayout::eUndefined;
	shadowDepthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::AttachmentReference shadowDepthAttachmentRef;
	shadowDepthAttachmentRef.attachment = 0;
	shadowDepthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

	vk::SubpassDescription shadowSubpass;
	shadowSubpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	shadowSubpass.colorAttachmentCount = 0;
	shadowSubpass.pDepthStencilAttachment = &shadowDepthAttachmentRef;

	vk::SubpassDependency shadowDependency;
	shadowDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	shadowDependency.dstSubpass = 0u;

	shadowDependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	shadowDependency.srcAccessMask = vk::AccessFlags();

	shadowDependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	shadowDependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

	std::array<vk::AttachmentDescription, 1> shadowAttachments = {shadowDepthAttachment};

	vk::RenderPassCreateInfo shadowRenderPassInfo;
	shadowRenderPassInfo.attachmentCount = shadowAttachments.size();
	shadowRenderPassInfo.pAttachments = shadowAttachments.data();
	shadowRenderPassInfo.subpassCount = 1;
	shadowRenderPassInfo.pSubpasses = &shadowSubpass;
	shadowRenderPassInfo.dependencyCount = 1;
	shadowRenderPassInfo.pDependencies = &shadowDependency;

	pipelines[0].renderPass = device->createRenderPass(shadowRenderPassInfo, nullptr, deviceLoader);

	// COLOR PIPELINE -----------------------------------------------------------------------------------------------

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
	dependency.dstSubpass = 0u;

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

	pipelines[1].renderPass = device->createRenderPass(renderPassInfo, nullptr, deviceLoader);

}

void Render::createDescriptorSetLayout() {

	uint32_t binding = 0;

	// UBO
	vk::DescriptorSetLayoutBinding MVPLayoutBinding;
	MVPLayoutBinding.binding = binding++;
	MVPLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	MVPLayoutBinding.descriptorCount = 1;
	MVPLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;

	std::vector<vk::DescriptorSetLayoutBinding> shadowBindings;
	shadowBindings.push_back(MVPLayoutBinding);

	vk::DescriptorSetLayoutCreateInfo shadowLayoutInfo;
	shadowLayoutInfo.bindingCount = shadowBindings.size();
	shadowLayoutInfo.pBindings = shadowBindings.data();

	pipelines[0].descriptorSetLayout = device->createDescriptorSetLayout(shadowLayoutInfo, nullptr, deviceLoader);

	// Light UBO
	vk::DescriptorSetLayoutBinding lightLayoutBinding;
	lightLayoutBinding.binding = binding++;
	lightLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
	lightLayoutBinding.descriptorCount = 1;
	lightLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;

	// Sampler
	vk::DescriptorSetLayoutBinding samplerLayoutBinding;
	samplerLayoutBinding.binding = binding++;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	// Shadowmap sampler
	vk::DescriptorSetLayoutBinding shadowMapLayoutBinding;
	shadowMapLayoutBinding.binding = binding++;
	shadowMapLayoutBinding.descriptorCount = 1;
	shadowMapLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
	shadowMapLayoutBinding.pImmutableSamplers = nullptr;
	shadowMapLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;

	std::vector<vk::DescriptorSetLayoutBinding> bindings;
	bindings.push_back(MVPLayoutBinding);
	bindings.push_back(lightLayoutBinding);
	bindings.push_back(samplerLayoutBinding);
	bindings.push_back(shadowMapLayoutBinding);

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.bindingCount = bindings.size();
	layoutInfo.pBindings = bindings.data();

	pipelines[1].descriptorSetLayout = device->createDescriptorSetLayout(layoutInfo, nullptr, deviceLoader);

}

void Render::createGraphicsPipeline() {

	// Common attributes

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	// Primitive style
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// First Pipeline, "sun point of view"
	{
		Shader vert(device.get(), vk::ShaderStageFlagBits::eVertex, "data/shaders/depthonly.vert.spv", deviceLoader);

		vk::PipelineShaderStageCreateInfo depthonlyShaderInfo = vert.getShaderStageInfo();

		// Input of the vertex buffer
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = 1u;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// Viewport
		vk::Viewport viewport;
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = 2048.f;
		viewport.height = 2048.f;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.0f;

		// Scissor
		vk::Rect2D scissor;
		scissor.offset = vk::Offset2D();
		scissor.extent = vk::Extent2D(2048u, 2048u);

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

		// Dynamic state
		// Allows to change some parameters without recreating the whole pipeline
		// See DynamicState to know the ones that can be dynamically set

		// Pipeline Layout
		vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &pipelines[0].descriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		pipelines[0].pipelineLayout = device->createPipelineLayout(pipelineLayoutInfo, nullptr, deviceLoader);

		// Graphics Pipeline
		vk::GraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.stageCount = 1;
		pipelineInfo.pStages = &depthonlyShaderInfo;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = nullptr;
		pipelineInfo.pDynamicState = nullptr;

		pipelineInfo.layout = pipelines[0].pipelineLayout;

		pipelineInfo.renderPass = pipelines[0].renderPass;
		pipelineInfo.subpass = 0;

		pipelines[0].pipeline = device->createGraphicsPipeline(nullptr, pipelineInfo, nullptr, deviceLoader);

		pipelines[0].depth = createDepthResources(2048u, 2048u, vk::ImageUsageFlagBits::eSampled);
	}

	// Second Pipeline, the one drawing something on the screen
	{
		Shader vert(device.get(), vk::ShaderStageFlagBits::eVertex, "data/shaders/shadow.vert.spv", deviceLoader);
		Shader frag(device.get(), vk::ShaderStageFlagBits::eFragment, "data/shaders/shadow.frag.spv", deviceLoader);

		// Create the actuals shaders and link them
		vk::PipelineShaderStageCreateInfo vertShaderStageInfo = vert.getShaderStageInfo();

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo = frag.getShaderStageInfo();

		vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

		// Input of the vertex buffer
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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
		pipelineLayoutInfo.pSetLayouts = &pipelines[1].descriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		pipelines[1].pipelineLayout = device->createPipelineLayout(pipelineLayoutInfo, nullptr, deviceLoader);

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

		pipelineInfo.layout = pipelines[1].pipelineLayout;

		pipelineInfo.renderPass = pipelines[1].renderPass;
		pipelineInfo.subpass = 0;

		pipelines[1].pipeline = device->createGraphicsPipeline(nullptr, pipelineInfo, nullptr, deviceLoader);

		pipelines[1].depth = createDepthResources(swapchain.getExtent().width, swapchain.getExtent().height);
	}
}

void Render::createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface.get(), deviceLoader);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	commandPool = device->createCommandPoolUnique(poolInfo, nullptr, deviceLoader);
}

void Render::createTexture() {
	Texture tex;
	tex.width = texture.width;
	tex.height = texture.height;
	tex.mipLevels = texture.mipLevels;

	texture = Texture(device.get(), deviceLoader);
	texture.width = tex.width;
	texture.height = tex.height;
	texture.mipLevels = tex.mipLevels;

	createImage(texture.width, texture.height, texture.mipLevels, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, texture.image, texture.memory);
}

void Render::createTextureView() {
	texture.view = createImageView(device.get(), texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageAspectFlagBits::eColor, texture.mipLevels, deviceLoader);
}

Texture Render::createDepthResources(uint32_t width, uint32_t height, vk::ImageUsageFlags otherUsages) {
	vk::Format depthFormat = findDepthFormat(physicalDevice, deviceLoader);

	Texture depth(device.get(), deviceLoader);
	depth.width = width;
	depth.height = height;
	depth.mipLevels = 1u;

	createImage(width, height, 1, depthFormat, vk::ImageTiling::eOptimal, otherUsages | vk::ImageUsageFlagBits::eDepthStencilAttachment, vk::MemoryPropertyFlagBits::eDeviceLocal, depth.image, depth.memory);

	depth.view = createImageView(device.get(), depth.image, depthFormat, vk::ImageAspectFlagBits::eDepth, 1, deviceLoader);

	return depth;
}

void Render::createFramebuffers() {
	uint32_t imageCount = swapchain.getImageCount();

	pipelines[0].framebuffers.resize(imageCount);
	for(size_t i = 0; i < imageCount; ++i) {
		std::vector<vk::ImageView> attachments;
		attachments.push_back(pipelines[0].depth.view);
		// (From vulkan-tutorial) Same depth image can be used because only a single subpass is running at the same time due to our semaphores
		// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.renderPass = pipelines[0].renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = pipelines[0].depth.width;
		framebufferInfo.height = pipelines[0].depth.height;
		framebufferInfo.layers = 1;

		pipelines[0].framebuffers[i] = device->createFramebuffer(framebufferInfo, nullptr, deviceLoader);
	}

	pipelines[1].framebuffers.resize(imageCount);
	for(size_t i = 0; i < imageCount; ++i) {
		std::vector<vk::ImageView> attachments;
		attachments.push_back(swapchain.getImageView(i));
		attachments.push_back(pipelines[1].depth.view);
		// (From vulkan-tutorial) Same depth image can be used because only a single subpass is running at the same time due to our semaphores
		// https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Framebuffers

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.renderPass = pipelines[1].renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchain.getExtent().width;
		framebufferInfo.height = swapchain.getExtent().height;
		framebufferInfo.layers = 1;

		pipelines[1].framebuffers[i] = device->createFramebuffer(framebufferInfo, nullptr, deviceLoader);
	}
}

void Render::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& memory) {
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	buffer = device->createBuffer(bufferInfo, nullptr, deviceLoader);

	auto memRequirements = device->getBufferMemoryRequirements(buffer, deviceLoader);

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties, deviceLoader);

	memory = device->allocateMemory(allocInfo, nullptr, deviceLoader);

	device->bindBufferMemory(buffer, memory, /*memoryOffset*/ 0, deviceLoader);
}

void Render::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Image& image, vk::DeviceMemory& imageMemory) {
	vk::ImageCreateInfo imageInfo;
	imageInfo.imageType = vk::ImageType::e2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = vk::ImageLayout::eUndefined;
	imageInfo.usage = usage;
	imageInfo.samples = vk::SampleCountFlagBits::e1;
	imageInfo.sharingMode = vk::SharingMode::eExclusive;

	image = device->createImage(imageInfo, nullptr, deviceLoader);

	vk::MemoryRequirements memRequirements = device->getImageMemoryRequirements(image, deviceLoader);

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties, deviceLoader);

	imageMemory = device->allocateMemory(allocInfo, nullptr, deviceLoader);

	device->bindImageMemory(image, imageMemory, /*memoryOffset*/0, deviceLoader);
}

void Render::createTextureSampler(float maxAnisotropy, float maxMipLod) {
	vk::SamplerCreateInfo samplerInfo;
	samplerInfo.magFilter = vk::Filter::eLinear;
	samplerInfo.minFilter = vk::Filter::eLinear;
	samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
	samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = maxAnisotropy;
	samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = vk::CompareOp::eAlways;
	samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = maxMipLod;

	textureSampler = device->createSamplerUnique(samplerInfo, nullptr, deviceLoader);
}

void Render::transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, uint32_t mipLevels) {
	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	if (oldLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal || newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
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
	else if (oldLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		srcStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
		dstStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else if (oldLayout == vk::ImageLayout::eShaderReadOnlyOptimal && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

		srcStage = vk::PipelineStageFlagBits::eFragmentShader;
		dstStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
	}
	else
		throw std::invalid_argument("Unsupported Layout transition !");

	commandBuffer.pipelineBarrier(srcStage, dstStage, vk::DependencyFlags(), nullptr, nullptr, barrier, deviceLoader);

	endSingleTimeCommands(commandBuffer);
}

void Render::createBuffers() {
	createBuffer(vertexBuffer.size, vk::BufferUsageFlagBits::eTransferDst | vertexBuffer.usage, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer.buffer, vertexBuffer.memory);
	createBuffer(indexBuffer.size, vk::BufferUsageFlagBits::eTransferDst | indexBuffer.usage, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer.buffer, indexBuffer.memory);
}

void Render::createUniformBuffers() {
	unsigned int swapChainImageCount = swapchain.getImageCount();

	sunMVPUniformBuffers.resize(swapChainImageCount);
	for (unsigned int i = 0; i < swapChainImageCount; i++) {
		sunMVPUniformBuffers[i].usage = vk::BufferUsageFlagBits::eUniformBuffer;
		sunMVPUniformBuffers[i].size = sizeof(MVPUniformBufferObject);
		createBuffer(sunMVPUniformBuffers[i].size, sunMVPUniformBuffers[i].usage, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, sunMVPUniformBuffers[i].buffer, sunMVPUniformBuffers[i].memory);
	}

	viewMVPUniformBuffers.resize(swapChainImageCount);
	for (unsigned int image = 0; image < swapChainImageCount; image++) {
		viewMVPUniformBuffers[image].usage = vk::BufferUsageFlagBits::eUniformBuffer;
		viewMVPUniformBuffers[image].size = sizeof(MVPUniformBufferObject);
		createBuffer(viewMVPUniformBuffers[image].size, viewMVPUniformBuffers[image].usage, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, viewMVPUniformBuffers[image].buffer, viewMVPUniformBuffers[image].memory);
	}

	lightUniformBuffers.resize(swapChainImageCount);
	for (unsigned int i = 0; i < swapChainImageCount; i++) {
		lightUniformBuffers[i].usage = vk::BufferUsageFlagBits::eUniformBuffer;
		lightUniformBuffers[i].size = sizeof(LightUniformBufferObject);
		createBuffer(lightUniformBuffers[i].size, lightUniformBuffers[i].usage, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, lightUniformBuffers[i].buffer, lightUniformBuffers[i].memory);
	}

}

void Render::createUniformBuffer(vk::DeviceSize bufferSize, vk::Buffer& buffer, vk::DeviceMemory& memory) {
	createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, memory);
}

void Render::createDescriptorPool() {
	uint32_t swapChainImageCount = static_cast<uint32_t>(swapchain.getImageCount());

	std::array<vk::DescriptorPoolSize, 1> shadowPoolSizes;
	shadowPoolSizes[0].type = vk::DescriptorType::eUniformBuffer;
	shadowPoolSizes[0].descriptorCount = swapChainImageCount;	// Total count of this descriptor type in the pool

	vk::DescriptorPoolCreateInfo shadowCreateInfo;
	shadowCreateInfo.poolSizeCount = shadowPoolSizes.size();
	shadowCreateInfo.pPoolSizes = shadowPoolSizes.data();
	shadowCreateInfo.maxSets = swapChainImageCount;

	pipelines[0].descriptorPool = device->createDescriptorPool(shadowCreateInfo, nullptr, deviceLoader);

	std::array<vk::DescriptorPoolSize, 2> poolSizes;
	poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
	poolSizes[0].descriptorCount = swapChainImageCount * 2u;
	poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
	poolSizes[1].descriptorCount = swapChainImageCount * 2u;

	vk::DescriptorPoolCreateInfo createInfo;
	createInfo.poolSizeCount = poolSizes.size();
	createInfo.pPoolSizes = poolSizes.data();
	createInfo.maxSets = swapChainImageCount;

	pipelines[1].descriptorPool = device->createDescriptorPool(createInfo, nullptr, deviceLoader);
}

void Render::createDescriptorSets() {
	unsigned int swapChainImageCount = swapchain.getImageCount();

	{
		std::vector<vk::DescriptorSetLayout> layouts(swapChainImageCount, pipelines[0].descriptorSetLayout);
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = pipelines[0].descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
		allocInfo.pSetLayouts = layouts.data();

		pipelines[0].descriptorSets = device->allocateDescriptorSets(allocInfo, std::allocator<vk::DescriptorSet>(), deviceLoader);

		for (unsigned int image = 0; image < swapChainImageCount; image++) {

			vk::DescriptorBufferInfo bufferInfo;
			bufferInfo.buffer = sunMVPUniformBuffers[image].buffer;
			bufferInfo.offset = 0;
			bufferInfo.range = sunMVPUniformBuffers[image].size;

			vk::WriteDescriptorSet descriptorWrite;
			descriptorWrite.dstSet = pipelines[0].descriptorSets[image];
			descriptorWrite.dstBinding = 0u;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			device->updateDescriptorSets(descriptorWrite, nullptr, deviceLoader);
		}
	}
	{
		std::vector<vk::DescriptorSetLayout> layouts(swapChainImageCount, pipelines[1].descriptorSetLayout);
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = pipelines[1].descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
		allocInfo.pSetLayouts = layouts.data();

		pipelines[1].descriptorSets = device->allocateDescriptorSets(allocInfo, std::allocator<vk::DescriptorSet>(), deviceLoader);

		// transition depth texture before and after updating descriptor set
		transitionImageLayout(pipelines[0].depth.image, findDepthFormat(physicalDevice, deviceLoader), vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1u);
		transitionImageLayout(pipelines[0].depth.image, findDepthFormat(physicalDevice, deviceLoader), vk::ImageLayout::eDepthStencilAttachmentOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, 1u);

		for (unsigned int image = 0; image < swapChainImageCount; image++) {

			vk::DescriptorBufferInfo MVPBufferInfo;
			MVPBufferInfo.buffer = viewMVPUniformBuffers[image].buffer;
			MVPBufferInfo.offset = 0;
			MVPBufferInfo.range = viewMVPUniformBuffers[image].size;

			vk::WriteDescriptorSet MVPDescritporWrite;
			MVPDescritporWrite.dstSet = pipelines[1].descriptorSets[image];
			MVPDescritporWrite.dstBinding = 0u;
			MVPDescritporWrite.dstArrayElement = 0;
			MVPDescritporWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
			MVPDescritporWrite.descriptorCount = 1;
			MVPDescritporWrite.pBufferInfo = &MVPBufferInfo;

			device->updateDescriptorSets(MVPDescritporWrite, nullptr, deviceLoader);

			vk::DescriptorBufferInfo lightBufferInfo;
			lightBufferInfo.buffer = lightUniformBuffers[image].buffer;
			lightBufferInfo.offset = 0;
			lightBufferInfo.range = lightUniformBuffers[image].size;

			vk::WriteDescriptorSet lightDescritporWrite;
			lightDescritporWrite.dstSet = pipelines[1].descriptorSets[image];
			lightDescritporWrite.dstBinding = 1u;
			lightDescritporWrite.dstArrayElement = 0;
			lightDescritporWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
			lightDescritporWrite.descriptorCount = 1;
			lightDescritporWrite.pBufferInfo = &lightBufferInfo;

			device->updateDescriptorSets(lightDescritporWrite, nullptr, deviceLoader);

			vk::DescriptorImageInfo imageInfo;
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			imageInfo.imageView = texture.view;
			imageInfo.sampler = textureSampler.get();

			vk::WriteDescriptorSet textureDescriptorWrite;
			textureDescriptorWrite.dstSet = pipelines[1].descriptorSets[image];
			textureDescriptorWrite.dstBinding = 2u;
			textureDescriptorWrite.dstArrayElement = 0;
			textureDescriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			textureDescriptorWrite.descriptorCount = 1;
			textureDescriptorWrite.pImageInfo = &imageInfo;

			device->updateDescriptorSets(textureDescriptorWrite, nullptr, deviceLoader);

			vk::DescriptorImageInfo shadowImageInfo;
			shadowImageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			shadowImageInfo.imageView = pipelines[0].depth.view;
			shadowImageInfo.sampler = textureSampler.get();

			vk::WriteDescriptorSet shadowDescriptorWrite;
			shadowDescriptorWrite.dstSet = pipelines[1].descriptorSets[image];
			shadowDescriptorWrite.dstBinding = 3u;
			shadowDescriptorWrite.dstArrayElement = 0;
			shadowDescriptorWrite.descriptorType = vk::DescriptorType::eCombinedImageSampler;
			shadowDescriptorWrite.descriptorCount = 1;
			shadowDescriptorWrite.pImageInfo = &shadowImageInfo;

			device->updateDescriptorSets(shadowDescriptorWrite, nullptr, deviceLoader);
		}
		//transitionImageLayout(pipelines[0].depth.image, findDepthFormat(physicalDevice, deviceLoader), vk::ImageLayout::eShaderReadOnlyOptimal, vk::ImageLayout::eDepthStencilAttachmentOptimal, 1u);
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

void Render::generateMipmaps(vk::Image image, vk::Format imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

	// Check if the image format supports linear blitting
	vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(imageFormat, deviceLoader);
	if (!(formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear))
		throw std::runtime_error("Texture image format does not support linear blitting!");
		// In this case, could try to resize on CPU and upload each miplevelon the device memory

	vk::CommandBuffer commandBuffer = beginSingleTimeCommands();

	vk::ImageMemoryBarrier barrier;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
		barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, vk::DependencyFlags(), nullptr, nullptr, barrier, deviceLoader);

		vk::ImageBlit blit;
		blit.srcOffsets[0] = vk::Offset3D();
		blit.srcOffsets[1] = vk::Offset3D( mipWidth, mipHeight, 1 );
		blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = vk::Offset3D();
		blit.dstOffsets[1] = vk::Offset3D( mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 );
		blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		commandBuffer.blitImage(image, vk::ImageLayout::eTransferSrcOptimal, image, vk::ImageLayout::eTransferDstOptimal, blit, vk::Filter::eLinear, deviceLoader);

		barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
		barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlagBits(), nullptr, nullptr, barrier, deviceLoader);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
	barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
	barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
	barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

	commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, vk::DependencyFlags(), nullptr, nullptr, barrier, deviceLoader);

	endSingleTimeCommands(commandBuffer);
}

vk::CommandBuffer Render::beginSingleTimeCommands() {
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool.get();
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = 1;

	auto commandBuffer = device->allocateCommandBuffers(allocInfo, deviceLoader)[0];

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

	graphicQueue.submit(submitInfo, nullptr, deviceLoader);
	graphicQueue.waitIdle(deviceLoader);

	device->freeCommandBuffers(commandPool.get(), commandBuffer, deviceLoader);
}

void Render::createCommandBuffers() {
	commandBuffers.resize(swapchain.getImageCount());

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool.get();
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	commandBuffers = device->allocateCommandBuffersUnique(allocInfo, std::allocator<vk::UniqueHandle<vk::CommandBuffer, vk::DispatchLoaderDynamic>>(), deviceLoader);

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		vk::CommandBufferBeginInfo beginInfo;
		beginInfo.pInheritanceInfo = nullptr;

		commandBuffers[i]->begin(beginInfo, deviceLoader);
		{
			// Shadow pass : compute shadowmap
			vk::RenderPassBeginInfo renderPassInfo;
			renderPassInfo.renderPass = pipelines[0].renderPass;
			renderPassInfo.framebuffer = pipelines[0].framebuffers[i];
			renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
			renderPassInfo.renderArea.extent = vk::Extent2D(2048u, 2048u);
			std::array<vk::ClearValue, 1> clearColors;
			clearColors[0].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
			renderPassInfo.clearValueCount = clearColors.size();
			renderPassInfo.pClearValues = clearColors.data();

			commandBuffers[i]->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline, deviceLoader);

			commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines[0].pipeline, deviceLoader);

			vk::Buffer vertexBuffers[1] = {vertexBuffer.buffer};
			vk::Buffer indexBuffer = this->indexBuffer.buffer;
			uint32_t indexCount = this->indexBuffer.size / sizeof(uint32_t);

			vk::DeviceSize offsets[1] = {0};

			commandBuffers[i]->bindVertexBuffers(/*first*/0, /*count*/1, vertexBuffers, offsets, deviceLoader);
			commandBuffers[i]->bindIndexBuffer(indexBuffer, /*offset*/0, vk::IndexType::eUint32, deviceLoader);

			commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelines[0].pipelineLayout, /*first set*/0, pipelines[0].descriptorSets[i], nullptr, deviceLoader);

			commandBuffers[i]->drawIndexed(indexCount, /*instance count*/1, /*first index*/0, /*vertex offset*/0, /*first instance*/0, deviceLoader);

			commandBuffers[i]->endRenderPass(deviceLoader);
		}
		{
			// transition depth image from pipeline 0 to a readonly layout for pipeline 1
			vk::ImageMemoryBarrier barrier;
			barrier.oldLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
			barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = pipelines[0].depth.image;
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
			if (hasStencilComponent(findDepthFormat(physicalDevice, deviceLoader)))
				barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;

			vk::PipelineStageFlags srcStage;
			vk::PipelineStageFlags dstStage;

			barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
			barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

			srcStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
			dstStage = vk::PipelineStageFlagBits::eFragmentShader;

			commandBuffers[i]->pipelineBarrier(srcStage, dstStage, vk::DependencyFlags(), nullptr, nullptr, barrier, deviceLoader);

			// Color pass : draw mesh with shadows
			vk::RenderPassBeginInfo renderPassInfo;
			renderPassInfo.renderPass = pipelines[1].renderPass;
			renderPassInfo.framebuffer = pipelines[1].framebuffers[i];
			renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
			renderPassInfo.renderArea.extent = swapchain.getExtent();
			std::array<vk::ClearValue, 2> clearColors;
			clearColors[0].color = vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.f}));
			clearColors[1].depthStencil = vk::ClearDepthStencilValue(1.0f, 0.0f);
			renderPassInfo.clearValueCount = clearColors.size();
			renderPassInfo.pClearValues = clearColors.data();

			commandBuffers[i]->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline, deviceLoader);

			commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines[1].pipeline, deviceLoader);

			vk::Buffer vertexBuffers[1] = {vertexBuffer.buffer};
			vk::Buffer indexBuffer = this->indexBuffer.buffer;
			uint32_t indexCount = this->indexBuffer.size / sizeof(uint32_t);

			vk::DeviceSize offsets[1] = {0};

			commandBuffers[i]->bindVertexBuffers(/*first*/0, /*count*/1, vertexBuffers, offsets, deviceLoader);
			commandBuffers[i]->bindIndexBuffer(indexBuffer, /*offset*/0, vk::IndexType::eUint32, deviceLoader);

			commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelines[1].pipelineLayout, /*first set*/0, pipelines[1].descriptorSets[i], nullptr, deviceLoader);

			commandBuffers[i]->drawIndexed(indexCount, /*instance count*/1, /*first index*/0, /*vertex offset*/0, /*first instance*/0, deviceLoader);

			commandBuffers[i]->endRenderPass(deviceLoader);
		}
		commandBuffers[i]->end(deviceLoader);
	}
}

void Render::createSemaphores() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

	vk::SemaphoreCreateInfo createInfo;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		imageAvailableSemaphores[i] = device->createSemaphoreUnique(createInfo, nullptr, deviceLoader);
		renderFinishedSemaphores[i] = device->createSemaphoreUnique(createInfo, nullptr, deviceLoader);
	}
}

void Render::createFences() {
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	vk::FenceCreateInfo createInfo;
	createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		inFlightFences[i] = device->createFenceUnique(createInfo, nullptr, deviceLoader);
}
