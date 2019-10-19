#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <chrono>

#include "WindowedApp.hpp"
#include "RenderPipeline.hpp"
#include "Render.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using std::chrono::high_resolution_clock;

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Tutorial : public WindowedApp<Render> {

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	unsigned int uniformIndex;

	unsigned int textureIndex;

	unsigned int vertexBufferIndex;
	unsigned int indexBufferIndex;

	RenderPipeline pipeline;

	const uint uniformLocation = 0u;
	const uint textureSamplerLocation = 1u;

public:

	Tutorial() : WindowedApp("Vulkan", VK_MAKE_VERSION(0, 0, 0)) {
		window.setKeyCallback(
			[&](Key key, int scancode, int action, KeyModifiersFlags modifiers) {
				if (key == Key::eEscape && action == GLFW_PRESS) {
					window.close();
				}
			}
		);
	}

	~Tutorial() {

	}

	/** Called first and once after Application ctor */
	void init() {

		// Add windows extensions to render to be able to draw in the window
		auto extensions = window.getRequiredExtensions();
		for (auto extensionName : extensions)
			render.addInstanceExtension(extensionName);
		render.addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		// initialise render
		render.init();

		// Add a pipeline
		render.addPipeline(pipeline);
		
		// set vertex and fragment shaders
		pipeline.setVertexShader("data/shaders/shader.vert.spv");
		pipeline.setFragmentShader("data/shaders/shader.frag.spv");
		pipeline.setViewportSize(vk::Extent2D(window.getWidth(), window.getHeight()));

		pipeline.enableDepthTest();

		// Add one uniform and one sampler descriptors
		pipeline.addUniform<UniformBufferObject>(vk::ShaderStageFlagBits::eVertex, uniformLocation);
		pipeline.addSampler(vk::ShaderStageFlagBits::eFragment, textureSamplerLocation);	

		// mesh and texture filenames
		const std::string meshFilename = "data/models/chalet.obj";
		const std::string meshTextureFilename = "data/textures/chalet.jpg";

		loadMesh(meshFilename);

		std::cout << "Mesh vertices count : " << vertices.size() << "\n";
		std::cout << "Mesh triangles count: " << indices.size() / 3 << "\n";

		// Mesh buffers
		vertexBufferIndex = render.addBuffer(vertices.size() * sizeof(vertices[0]), vk::BufferUsageFlagBits::eVertexBuffer);
		indexBufferIndex  = render.addBuffer(indices.size() * sizeof(indices[0]), vk::BufferUsageFlagBits::eIndexBuffer);

		textureIndex = render.addTexture(meshTextureFilename);

		render.finishSetup();

		// render have created actual buffers, we can copy data into it
		render.fillBuffer(vertexBufferIndex, vertices);
		render.fillBuffer(indexBufferIndex, indices);

	}

	/** Load the mesh and forward it to the render */
	void loadMesh(const std::string& filename) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
			throw std::runtime_error(warn + err);

		std::unordered_map<Vertex, uint32_t> uniqueVertices;

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				Vertex vertex;
				vertex.pos = {
					attrib.vertices[3 * index.vertex_index+0],
					attrib.vertices[3 * index.vertex_index+1],
					attrib.vertices[3 * index.vertex_index+2]
				};
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index+0],
					1.f - attrib.texcoords[2 * index.texcoord_index+1]
				};
				vertex.color = {1.f, 1.f, 1.f};

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	/** Called once at the program exit to handle resource destruction */
	void quit() {

	}

	/** Called at each frame before draw */
	void update() {
		window.pollEvents();

		static auto startTime = high_resolution_clock::now();
		static auto lastTime = high_resolution_clock::now();
		static auto updateTime = high_resolution_clock::now();

		auto currentTime = high_resolution_clock::now();

		// print FPS and frametime
		float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastTime).count();
		float framesPerSecond = (1 / frameTime) * 1000;
		std::string title("vulkan-tutorial FPS : ");
		if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - updateTime) > std::chrono::milliseconds(2000)) {
			window.setTitle(title + std::to_string(framesPerSecond));
			updateTime = currentTime;
		}

		lastTime = currentTime;

		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), window.getWidth() / (float)window.getHeight(), 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;	// because of OpenGL upside down screen coordinates

		//render.updateUniform(uniformIndex, &ubo, sizeof(ubo));
		pipeline.updateUniform(ubo, uniformLocation);

	}

	/** Return true to continue, false to end the main loop and exit the program */
	bool draw() {
		const auto& deviceLoader = render.getDeviceLoader();

		// record Command Buffer here
		vk::CommandBuffer commandBuffer = render.beginDrawCommands();

		auto renderPassInfo = render.getRenderPassInfo(pipeline);

		commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline, deviceLoader);

		render.bindPipeline(commandBuffer, pipeline);

		vk::Buffer vertexBuffers[1] = { render.getBuffer(vertexBufferIndex) };
		vk::Buffer indexBuffer = render.getBuffer(indexBufferIndex);
		uint32_t indexCount = indices.size();

		vk::DeviceSize offsets[1] = {0};

		commandBuffer.bindVertexBuffers(/*first*/0, /*count*/1, vertexBuffers, offsets, deviceLoader);
		commandBuffer.bindIndexBuffer(indexBuffer, /*offset*/0, vk::IndexType::eUint32, deviceLoader);

		render.bindDescritpor(commandBuffer, pipeline);

		//commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.pipelineLayout, /*first set*/0, pipeline.descriptorSets[i], nullptr, deviceLoader);

		commandBuffer.drawIndexed(indexCount, /*instance count*/1, /*first index*/0, /*vertex offset*/0, /*first instance*/0, deviceLoader);

		commandBuffer.endRenderPass(deviceLoader);

		render.endDrawCommands(commandBuffer);

		render.submitDrawCommands(commandBuffer);

		// TODO : simplify all this with a function call like render.draw(pipeline, model);
		// TODO : next, pack the models in a scene object

		return !window.isClosed();
	}

};

int main() {
	try {
		Tutorial app;
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
