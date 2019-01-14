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
	vk::Device _device;

	Shader(const std::string& file, vk::Device device) : path(file), buffer(readFile(path)), _device(device) {
		vk::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
		shaderModule = _device.createShaderModule(createInfo);
	}

	~Shader() {
		_device.destroyShaderModule(shaderModule);
	}
};

#endif
