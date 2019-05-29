#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>

#include "fileUtils.hpp"
#include "vulkan/vulkan.hpp"

struct Shader {
	std::string path;
	std::vector<char> buffer;
	vk::ShaderModule shaderModule;
	vk::Device device;
	vk::ShaderStageFlagBits stage;

	Shader(vk::Device device, vk::ShaderStageFlagBits stage) : device(device), stage(stage) {

	}

	~Shader() {
		device.destroyShaderModule(shaderModule);
	}

	void create(const std::string& file) {
		path = file;
		buffer = readFile(path);
		vk::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
		shaderModule = device.createShaderModule(createInfo);
	}

	vk::PipelineShaderStageCreateInfo getShaderStageInfo() {
		vk::PipelineShaderStageCreateInfo shaderStageInfo;
		shaderStageInfo.stage = stage;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";		// entry point in the shader's code
		return shaderStageInfo;
	}
};

#endif
