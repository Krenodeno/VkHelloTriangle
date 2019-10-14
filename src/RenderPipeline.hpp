#ifndef RENDER_PIPELINE_HPP
#define RENDER_PIPELINE_HPP

#include <string>

#include "Vulkan.hpp"

#include "Texture.hpp"

// Forward declaration
class Render;

class RenderPipeline {
public:

	RenderPipeline();
	RenderPipeline(vk::Device, vk::DispatchLoaderDynamic);
	~RenderPipeline();

	void cleanup();

	void enableDepthTest() { depthParams.enableDepthTest = true; }
	void disableDepthTest() { depthParams.enableDepthTest = false; }

	template<typename T>
	void addUniform(vk::ShaderStageFlags stage, uint32_t location) {
		descriptors.push_back({stage, vk::DescriptorType::eUniformBuffer, location, sizeof(T)});
		uniformData.push_back(new T);
	}

	template<typename T>
	void updateUniform(const T& data, uint32_t location) {
		// Find the right descriptor by its location id
		uint32_t uniformDataIndex = [&](){
			uint32_t index = 0u;
			for (int i = 0; i < descriptors.size(); i++) {
				if (location == descriptors[i].location)
					return index;
				if (descriptors[i].type == vk::DescriptorType::eUniformBuffer)
					index++;
			}
			return ~0u;
		}();

		*(static_cast<T*>(uniformData[uniformDataIndex])) = data;
	}

	void addSampler(vk::ShaderStageFlags stage, uint32_t location) {
		descriptors.push_back({stage, vk::DescriptorType::eCombinedImageSampler, location});
	}

	void setVertexShader(std::string filename) { vertexShaderFileName = filename; }
	void setTessellationShader(std::string filename) { tessellationShaderFileName = filename; }
	void setGeometryShader(std::string filename) { geometryShaderFileName = filename; }
	void setFragmentShader(std::string filename) { fragmentShaderFileName = filename; }

	void setViewportSize(vk::Extent2D size) { viewportSize = size; }

private:
	vk::RenderPass renderPass;
	vk::DescriptorSetLayout descriptorSetLayout;
	vk::DescriptorPool descriptorPool;
	std::vector<vk::DescriptorSet> descriptorSets;
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;

	vk::Device device;
	vk::DispatchLoaderDynamic deviceLoader;

	// RenderPass parameters
	struct DepthParameters {
		vk::Bool32 enableDepthTest;
	} depthParams;

	// DescriptorSetLayout parameters
	struct Descriptor {
		vk::ShaderStageFlags stage;
		vk::DescriptorType   type;
		uint32_t			 location;
		uint64_t			 bufferSize;
		vk::ImageView		 imageView;
	};
	std::vector<Descriptor> descriptors;
	std::vector<void*> uniformData; // TODO find a better way to hold any type of uniform data

	// Pipeline parameters
	// shaders
	std::string vertexShaderFileName;
	std::string tessellationShaderFileName;
	std::string geometryShaderFileName;
	std::string fragmentShaderFileName;
	// Viewport
	vk::Extent2D viewportSize;
	// Multisampling
	vk::SampleCountFlags sampleCount;
	// Depth buffer
	Texture depthBuffer;

	void createRenderPass(vk::Format color, vk::Format depth);
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createDescriptorPool(uint32_t swapChainImageCount);
	// in general, you want to create as much descriptor set as images in the swapchain
	void createDescriptorSets(uint32_t descriptorSetCount);

	friend Render;
};

#endif
