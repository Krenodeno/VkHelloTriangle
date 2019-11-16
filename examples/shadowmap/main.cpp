#include "WindowedApp.hpp"
#include "Render.hpp"

#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class ShadowMapExample : public WindowedApp<Render> {
public:

	ShadowMapExample() : WindowedApp("shadowMap", VK_MAKE_VERSION(0, 0, 0), 1024, 768) {
		window.setKeyCallback(
			[&](Key key, int scancode, int action, KeyModifiersFlags modifiers) {
				if (key == Key::eEscape && action == GLFW_PRESS) {
					window.close();
				}
			}
		);
		window.setResizeCallback(
			[&](int width, int height) {
				render.setExtent(vk::Extent2D(width, height));
			}
		);
	}

	~ShadowMapExample() {

	}

	/** Called first and once after Application ctor */
	void init() {
		auto extensions = window.getRequiredExtensions();
		for (auto extensionName : extensions)
			render.addInstanceExtension(extensionName);
		render.addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		loadMesh("data/models/chalet.obj", vertices, indices);

		render.setVertexBufferSize(vertices.size() * sizeof(Vertex));
		render.setIndexBufferSize(indices.size() * sizeof(uint32_t));

		int texWidth, texHeight, texChannels;
		auto pixels = loadTexture("data/textures/chalet.jpg", texWidth, texHeight, texChannels);
		if (!pixels)
			throw std::runtime_error("Could not load texture!");

		render.setTextureSize(texWidth, texHeight);

		// initialise render
		render.init();

		render.fillBuffer(render.getVertexBuffer(), vertices.data(), vertices.size() * sizeof(Vertex));
		render.fillBuffer(render.getIndexBuffer(), indices.data(), indices.size() * sizeof(uint32_t));

		render.fillTexture(pixels);

		stbi_image_free(pixels);
	}

	/** Called once at the program exit to handle resource destruction */
	void quit() {

	}

	/** Called at each frame before draw */
	void update() {

	}

	/** Return true to continue, false to end the main loop and exit the program */
	bool draw() {
		render.drawFrame();
		return !window.isClosed();
	}

	/** Load the mesh */
	void loadMesh(const std::string& filename, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {

		assert(vertices.empty() && indices.empty());

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

	stbi_uc* loadTexture(const std::string& filename, int& texWidth, int& texHeight, int& texChannels) {
		return stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	}

private:


};

int main(int argc, char* argv[]) {
	try {
		ShadowMapExample app;
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;

}