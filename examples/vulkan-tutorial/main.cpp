#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <chrono>

#include "WindowedApp.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

using std::chrono::high_resolution_clock;

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Tutorial : public WindowedApp {

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

public:

	Tutorial() : WindowedApp("Vulkan", VK_MAKE_VERSION(0, 0, 0)) {
		render.setSurfaceCreationFunction(
			[&](vk::Instance instance, vk::DispatchLoaderDynamic d) {
				return window.createSurface(instance, d);
			}
		);

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

		// set vertex and fragment shaders
		render.setVertexShader("data/shaders/shader.vert.spv");
		render.setFragmentShader("data/shaders/shader.frag.spv");

		// mesh and texture filenames
		const std::string meshFilename = "data/models/chalet.obj";
		const std::string meshTextureFilename = "data/textures/chalet.jpg";

		loadMesh(meshFilename);

		std::cout << "Mesh vertices count : " << vertices.size() << "\n";
		std::cout << "Mesh triangles count: " << indices.size() / 3 << "\n";

		unsigned int vertexBuffer = render.addBuffer(vertices.size() * sizeof(vertices[0]), vk::BufferUsageFlagBits::eVertexBuffer);
		unsigned int indexBuffer = render.addBuffer(indices.size() * sizeof(indices[0]), vk::BufferUsageFlagBits::eIndexBuffer);

		render.addTexture(meshTextureFilename);

		unsigned int uniformBuffer = render.addUniform(sizeof(UniformBufferObject), vk::ShaderStageFlagBits::eVertex);

		// Initialise the render with previous info set
		render.init();	// TODO replace with wanted functions calls to initialize the render

		// render have created actual buffers, we can copy data into it
		render.fillBuffer<Vertex>(vertexBuffer, vertices);
		render.fillBuffer<uint32_t>(indexBuffer, indices);

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

		auto currentTime = high_resolution_clock::now();

		float frameTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastTime).count();
		std::cout << "Frame time = " << frameTime << "ms\r";
		lastTime = currentTime;

		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), window.getWidth() / (float)window.getHeight(), 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;	// because of OpenGL upside down screen coordinates

		render.updateUniform(0, &ubo, sizeof(ubo));

	}

	/** Return true to continue, false to end the main loop and exit the program */
	bool draw() {
		render.drawFrame();

		return !window.isClosed();
	}

};

int main() {
	Tutorial app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
