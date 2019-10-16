#include "RenderPipeline.hpp"

#include "Shader.hpp"
#include "Vertex.hpp"

RenderPipeline::RenderPipeline() : sampleCount(vk::SampleCountFlagBits::e1) {

}

RenderPipeline::RenderPipeline(vk::Device dev, vk::DispatchLoaderDynamic dispatch) : device(dev), deviceLoader(dispatch), sampleCount(vk::SampleCountFlagBits::e1) {

}

RenderPipeline::~RenderPipeline() {
    cleanup();
}

void RenderPipeline::cleanup() {
	if (pipeline){
        device.destroyPipeline(pipeline, nullptr, deviceLoader);
		pipeline = nullptr;
	}
    if (pipelineLayout) {
		device.destroyPipelineLayout(pipelineLayout, nullptr, deviceLoader);
		pipelineLayout = nullptr;
	}
	if (descriptorPool) {
		device.destroyDescriptorPool(descriptorPool, nullptr, deviceLoader);
		descriptorPool = nullptr;
	}
    if (descriptorSetLayout) {
        device.destroyDescriptorSetLayout(descriptorSetLayout, nullptr, deviceLoader);
		descriptorSetLayout = nullptr;
	}
    if (renderPass) {
        device.destroyRenderPass(renderPass, nullptr, deviceLoader);
		renderPass = nullptr;
	}
	
	for (int i = 0; i < uniformData.size(); i++) {
		void*& data = uniformData[i];
		delete data;
		data = nullptr;
	}
	uniformData.clear();
}

void RenderPipeline::createRenderPass(vk::Format colorFormat, vk::Format depthFormat = vk::Format::eUndefined) {
    vk::AttachmentDescription colorAttachment;
	colorAttachment.format = colorFormat;
	colorAttachment.samples = vk::SampleCountFlagBits::e1;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef;
	colorAttachmentRef.attachment = 0;	// layout(location = 0)
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::AttachmentDescription depthAttachment;
	depthAttachment.format = depthFormat;
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
	if (depthParams.enableDepthTest) 
		subpass.pDepthStencilAttachment = &depthAttachmentRef;
	else
		subpass.pDepthStencilAttachment = nullptr;
	

	vk::SubpassDependency dependency;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;	// no subpass dependency
	dependency.dstSubpass = 0u;

	dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.srcAccessMask = vk::AccessFlags();

	dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

	std::vector<vk::AttachmentDescription> attachments;
	attachments.push_back(colorAttachment);
	if (depthParams.enableDepthTest)
		attachments.push_back(depthAttachment);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	renderPass = device.createRenderPass(renderPassInfo, nullptr, deviceLoader);
}

void RenderPipeline::createDescriptorSetLayout() {
    std::vector<vk::DescriptorSetLayoutBinding> bindings;

	for (unsigned int i = 0; i < descriptors.size(); i++) {
		auto descriptor = descriptors[i];

		vk::DescriptorSetLayoutBinding layoutBinding;
		layoutBinding.binding = i;
		layoutBinding.descriptorType = descriptor.type;
		layoutBinding.descriptorCount = 1;
		layoutBinding.stageFlags = descriptor.stage;

		bindings.push_back(layoutBinding);
	}

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.bindingCount = bindings.size();
	layoutInfo.pBindings = bindings.data();

	descriptorSetLayout = device.createDescriptorSetLayout(layoutInfo, nullptr, deviceLoader);
}

void RenderPipeline::createGraphicsPipeline() {
    // TODO create shader only if filenames are provided
    Shader vert(device, vk::ShaderStageFlagBits::eVertex, vertexShaderFileName, deviceLoader);
	Shader frag(device, vk::ShaderStageFlagBits::eFragment, fragmentShaderFileName, deviceLoader);

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
	viewport.width = static_cast<float>(viewportSize.width);
	viewport.height = static_cast<float>(viewportSize.height);
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.f;

	// Scissor
	vk::Rect2D scissor;
	scissor.offset = vk::Offset2D(0, 0);
	scissor.extent = viewportSize;

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
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

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

	pipeline = device.createGraphicsPipeline(nullptr, pipelineInfo, nullptr, deviceLoader);
}

void RenderPipeline::createDescriptorPool(uint32_t swapChainImageCount) {
    std::vector<vk::DescriptorPoolSize> poolSizes;
    for (int i = 0; i < descriptors.size(); i++) {
        vk::DescriptorPoolSize size;
        size.type = descriptors[i].type;
        size.descriptorCount = swapChainImageCount;
        poolSizes.push_back(size);
    }

	vk::DescriptorPoolCreateInfo createInfo;
	createInfo.poolSizeCount = poolSizes.size();
	createInfo.pPoolSizes = poolSizes.data();
	createInfo.maxSets = swapChainImageCount;

	descriptorPool = device.createDescriptorPool(createInfo, nullptr, deviceLoader);
}

void RenderPipeline::createDescriptorSets(uint32_t descriptorSetCount) {
	std::vector<vk::DescriptorSetLayout> layouts(descriptorSetCount, descriptorSetLayout);
	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = descriptorSetCount;
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets = device.allocateDescriptorSets(allocInfo, std::allocator<vk::DescriptorSet>(), deviceLoader);

    // Update descriptors now, or before drawing ?
	// Now : need to know the uniform buffer and its size
	// as well as ImageView
}
