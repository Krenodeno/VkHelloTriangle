#include "Render.hpp"

#include "fileUtils.hpp"

#include <ostream>
#include <set>

Render::Render() : renderType(RenderTypeBits::eGraphics), validationLayerEnabled(false) {

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
	createGraphicsPipeline();
	createFramebuffers();
	createCommandPool();
	createVertexBuffer();
	createIndexBuffer();
	createCommandBuffers();
	createSemaphores();
	createFences();
}

void Render::cleanupSwapchain() {

	device.freeCommandBuffers(commandPool, commandBuffers, dispatchLoader);

	device.destroyPipeline(graphicsPipeline, nullptr, dispatchLoader);
	device.destroyPipelineLayout(pipelineLayout, nullptr, dispatchLoader);
	device.destroyRenderPass(renderPass, nullptr, dispatchLoader);

}

void Render::cleanup() {
	// Destroy device related objects
	cleanupSwapchain();
	swapchain.cleanup();
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		device.destroyFence(inFlightFences[i], nullptr, dispatchLoader);
		device.destroySemaphore(renderFinishedSemaphores[i], nullptr, dispatchLoader);
		device.destroySemaphore(imageAvailableSemaphores[i], nullptr, dispatchLoader);
	}

	device.destroyBuffer(vertexBuffer, nullptr, dispatchLoader);
	device.free(vertexBufferMemory, nullptr, dispatchLoader);

	device.destroyBuffer(indexBuffer, nullptr, dispatchLoader);
	device.free(indexBufferMemory, nullptr, dispatchLoader);

	device.destroyCommandPool(commandPool, nullptr, dispatchLoader);

	// Destroy Device
	device.destroy(nullptr, dispatchLoader);
	// Destroy instance related objects
	instance.destroySurfaceKHR(surface, nullptr, dispatchLoader);
	vk::DispatchLoaderDynamic didy(instance);
	instance.destroyDebugUtilsMessengerEXT(callback, nullptr, didy);
	// Then destroy the instance
	instance.destroy();
}

void Render::recreateSwapChain() {
	device.waitIdle();

	cleanupSwapchain();

	swapchain.recreate(surface, physicalDevice, windowExtent);

	createRenderPass();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandBuffers();
}

void Render::drawFrame() {
	device.waitForFences(inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max(), dispatchLoader);

	uint32_t imageIndex;
	try {
		auto result = device.acquireNextImageKHR(swapchain.getSwapchain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], nullptr, dispatchLoader);
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

	device.resetFences(inFlightFences[currentFrame], dispatchLoader);

	graphicsQueue.submit(submitInfo, inFlightFences[currentFrame], dispatchLoader);

	vk::PresentInfoKHR presentInfo;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	vk::SwapchainKHR swapChains[] = { swapchain.getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	// TODO recreate swapchain when swapchain suboptimal too
	try {
		presentQueue.presentKHR(presentInfo, dispatchLoader);
	} catch (const vk::OutOfDateKHRError& e) {
		recreateSwapChain();
	}

	presentQueue.waitIdle(dispatchLoader);
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
	validationLayerEnabled = true;
	layers.push_back("VK_LAYER_LUNARG_standard_validation");
	instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

void Render::setParentApplication(Application* app) {
	parentApp = app;
}

void Render::setSurfaceCreationFunction(createSurfaceFoncter functer) {
	surfaceCreation = functer;
}

void Render::setExtent(const vk::Extent2D& extent) {
	windowExtent = extent;
}

void Render::createInstance() {

	std::cout << "Instance creation\n";
	std::cout << "Required layers: \n";
	for (auto layer : layers) {
		std::cout << "\t" << layer << "\t";
		if (!checkLayerSupport(layer)) {
			std::string error_msg1(layer);
			std::string error_msg2(" not supported !");
			throw std::runtime_error(error_msg1 + error_msg2);
		}
		std::cout << "\n";
	}

	std::cout << "Required extensions: \n";
	for (auto extension: instanceExtensions) {
		std::cout << "\t" << extension << (checkExtensionSupport(extension)?"\tsupported":"\tnon supported") << "\n";
	}

	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = "vulkan-tutorial";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "HelloTriangle";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = layers.size();
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledExtensionCount = instanceExtensions.size();
	instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();

	instance = vk::createInstance(instanceInfo, nullptr, dispatchLoader);
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

	callback = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, vk::DispatchLoaderDynamic(instance));
}

void Render::pickPhysicalDevice() {
	auto devices = instance.enumeratePhysicalDevices(dispatchLoader);

	if (devices.empty())
		throw std::runtime_error("Failed to find GPUs with Vulkan suport!");

	for (const auto& device: devices) {
		if (isDeviceSuitable(device, surface, deviceExtensions)) {
			physicalDevice = device;
			break;
		}
	}

	if (!physicalDevice)
		throw std::runtime_error("Failed to find a suitable GPU!");
}

void Render::createLogicalDevice() {
	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

	// Queues infos
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<unsigned int> uniqueFamilies = { indices.presentFamily.value() };
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

	device = physicalDevice.createDevice(createInfo, nullptr, dispatchLoader);

	if (renderType & RenderTypeBits::eGraphics)
		graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0, dispatchLoader);

	if (renderType & RenderTypeBits::eCompute)
		computeQueue = device.getQueue(indices.computeFamily.value(), 0, dispatchLoader);

	presentQueue = device.getQueue(indices.presentFamily.value(), 0, dispatchLoader);
}

void Render::createSurface() {
	surface = surfaceCreation(parentApp, instance);
}

void Render::createSwapchain(vk::SwapchainKHR oldSwapchain) {
	swapchain.init(surface, physicalDevice, device, windowExtent, oldSwapchain);
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

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	// index of colorAttachment is referenced in the fragment shader as layout(location = 0)

	vk::SubpassDependency dependency;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;

	dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.srcAccessMask = vk::AccessFlags();

	dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	renderPass = device.createRenderPass(renderPassInfo, nullptr, dispatchLoader);
}

void Render::createGraphicsPipeline() {
	// Get the bytecode
	auto vertModule = vert.createShaderModule("ressources/shaders/fixed.vert.spv", device);
	auto fragModule = frag.createShaderModule("ressources/shaders/fixed.frag.spv", device);

	// Create the actuals shaders and link them
	vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
	vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertShaderStageInfo.module = vertModule;
	vertShaderStageInfo.pName = "main";			// entry point in the shader's code

	vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
	fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragShaderStageInfo.module = fragModule;
	fragShaderStageInfo.pName = "main";			// entry point in the shader's code

	vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	// input of the vertex buffer
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	// Primitive style
	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//Viewport
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
	rasterizer.frontFace = vk::FrontFace::eClockwise;
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

	// Depth test
	// Stencil test

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
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo, nullptr, dispatchLoader);

	// Graphics Pipeline
	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr;

	pipelineInfo.layout = pipelineLayout;

	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;

	graphicsPipeline = device.createGraphicsPipeline(nullptr, pipelineInfo, nullptr, dispatchLoader);

	device.destroyShaderModule(vertModule, nullptr, dispatchLoader);
	device.destroyShaderModule(fragModule, nullptr, dispatchLoader);

}

void Render::createFramebuffers() {
	swapchain.createFramebuffers(renderPass);
}

void Render::createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	commandPool = device.createCommandPool(poolInfo, nullptr, dispatchLoader);
}

void Render::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer& buffer, vk::DeviceMemory& memory) {
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = vk::SharingMode::eExclusive;

	buffer = device.createBuffer(bufferInfo, nullptr, dispatchLoader);

	auto memRequirements = device.getBufferMemoryRequirements(buffer, dispatchLoader);

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	memory = device.allocateMemory(allocInfo, nullptr, dispatchLoader);

	device.bindBufferMemory(buffer, memory, /*memoryOffset*/ 0, dispatchLoader);
}

void Render::fillBuffer(vk::DeviceMemory& memory, const void* dataToCopy, vk::DeviceSize size) {
	auto data = device.mapMemory(memory, /*offset*/ 0, size, vk::MemoryMapFlags(), dispatchLoader);
		std::memcpy(data, dataToCopy, static_cast<size_t>(size));
	device.unmapMemory(memory, dispatchLoader);
}

void Render::createVertexBuffer() {
	vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);
	fillBuffer(stagingBufferMemory, vertices.data(), bufferSize);

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	device.freeMemory(stagingBufferMemory, nullptr, dispatchLoader);
	device.destroyBuffer(stagingBuffer, nullptr, dispatchLoader);
}

void Render::createIndexBuffer() {
	vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	vk::Buffer stagingBuffer;
	vk::DeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);
	fillBuffer(stagingBufferMemory, indices.data(), bufferSize);

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	device.freeMemory(stagingBufferMemory, nullptr, dispatchLoader);
	device.destroyBuffer(stagingBuffer, nullptr, dispatchLoader);
}

void Render::copyBuffer(vk::Buffer src, vk::Buffer dst, vk::DeviceSize size) {
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandBufferCount = 1;
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;

	vk::CommandBuffer commandBuffer = device.allocateCommandBuffers(allocInfo, dispatchLoader)[0];

	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

	commandBuffer.begin(beginInfo, dispatchLoader);

	vk::BufferCopy copyRegion;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	commandBuffer.copyBuffer(src, dst, copyRegion, dispatchLoader);

	commandBuffer.end(dispatchLoader);

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	graphicsQueue.submit(submitInfo, nullptr, dispatchLoader);
	graphicsQueue.waitIdle(dispatchLoader);

	device.freeCommandBuffers(commandPool, commandBuffer, dispatchLoader);
}

void Render::createCommandBuffers() {
	commandBuffers.resize(swapchain.getSize());

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = commandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	commandBuffers = device.allocateCommandBuffers(allocInfo, dispatchLoader);

	for (size_t i = 0; i < commandBuffers.size(); i++) {
		vk::CommandBufferBeginInfo beginInfo;
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;
		beginInfo.pInheritanceInfo = nullptr;

		commandBuffers[i].begin(beginInfo, dispatchLoader);

		vk::RenderPassBeginInfo renderPassInfo;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapchain.getFramebuffer(i);
		renderPassInfo.renderArea.offset = vk::Offset2D(0, 0);
		renderPassInfo.renderArea.extent = swapchain.getExtent();
		renderPassInfo.clearValueCount = 1;
		vk::ClearValue clearColor(vk::ClearColorValue(std::array<float, 4>({0.2f, 0.2f, 0.2f, 1.f})));
		renderPassInfo.pClearValues = &clearColor;

		commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline, dispatchLoader);

		commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline, dispatchLoader);

		vk::Buffer vertexBuffers[] = {vertexBuffer};
		vk::DeviceSize offsets[] = {0};

		commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets, dispatchLoader);
		commandBuffers[i].bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16, dispatchLoader);

		commandBuffers[i].drawIndexed(static_cast<uint32_t>(indices.size()), /*instance count*/1, 0, 0, 0, dispatchLoader);

		commandBuffers[i].endRenderPass(dispatchLoader);

		commandBuffers[i].end(dispatchLoader);
	}
}

void Render::createSemaphores() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);

	vk::SemaphoreCreateInfo createInfo;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		imageAvailableSemaphores[i] = device.createSemaphore(createInfo, nullptr, dispatchLoader);
		renderFinishedSemaphores[i] = device.createSemaphore(createInfo, nullptr, dispatchLoader);
	}
}

void Render::createFences() {
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	vk::FenceCreateInfo createInfo;
	createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		inFlightFences[i] = device.createFence(createInfo, nullptr, dispatchLoader);
}
