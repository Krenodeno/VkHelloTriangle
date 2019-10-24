#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "Vulkan.hpp"
#include "Texture.hpp"
#include <vector>

struct Pipeline {
	/* Vulkan object describing a pipeline in its globality */
	vk::RenderPass renderPass;
	vk::DescriptorSetLayout descriptorSetLayout;
	vk::DescriptorPool descriptorPool;
	std::vector<vk::DescriptorSet> descriptorSets;
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;
	Texture depth;
	std::vector<vk::Framebuffer> framebuffers;

	template<typename Dispatch>
	void cleanup(vk::Device device, Dispatch d) {
		for (auto framebuffer : framebuffers)
			device.destroy(framebuffer, nullptr, d);
		framebuffers.clear();
		if (pipeline) {
			device.destroy(pipeline, nullptr, d);
			pipeline = nullptr;
		}
		if (pipelineLayout) {
			device.destroy(pipelineLayout, nullptr, d);
			pipelineLayout = nullptr;
		}
		if (descriptorPool) {
			device.destroy(descriptorPool, nullptr, d);
			descriptorPool = nullptr;
		}
		if (descriptorSetLayout) {
			device.destroy(descriptorSetLayout, nullptr, d);
			descriptorSetLayout = nullptr;
		}
		if (renderPass) {
			device.destroy(renderPass, nullptr, d);
			renderPass = nullptr;
		}
	}
};

#endif