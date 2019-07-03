#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>

#include "fileUtils.hpp"
#include "Vulkan.hpp"

template<typename Dispatch = vk::DispatchLoaderDefault>
struct Shader {
	std::string path;
	std::vector<char> buffer;
	vk::ShaderModule shaderModule;
	const vk::Device& device;
	const Dispatch& deviceLoader;
	vk::ShaderStageFlagBits stage;

	Shader(const vk::Device& device, vk::ShaderStageFlagBits stage, const std::string& filename, const Dispatch& d) : device(device), deviceLoader(d), stage(stage) {
		createShaderModule(filename);
	}

	~Shader() {
		device.destroyShaderModule(shaderModule, nullptr, deviceLoader);
	}

	vk::PipelineShaderStageCreateInfo getShaderStageInfo() {
		vk::PipelineShaderStageCreateInfo shaderStageInfo;
		shaderStageInfo.stage = stage;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";		// entry point in the shader's code
		return shaderStageInfo;
	}

private:

	void createShaderModule(const std::string& file) {
		path = file;
		buffer = readFile(path);
		vk::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
		shaderModule = device.createShaderModule(createInfo, nullptr, deviceLoader);
	}

};

#endif
