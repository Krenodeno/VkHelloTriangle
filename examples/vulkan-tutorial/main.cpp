#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "Application.hpp"
#include "RenderWindow.hpp"

class Tutorial : public Application {
public:

	Tutorial() : Application("Vulkan", VK_MAKE_VERSION(0, 0, 0)),
				 window(800, 600, "VkHelloTriangle") {
		render.setSurfaceCreationFunction(
			[&](vk::Instance instance) {
				return window.createSurface(instance);
			}
		);
	}

	~Tutorial() {

	}

	/** Have to be derivated */
	void init() {

		// Add windows extensions to render to be able to draw in the window
		auto extensions = window.getRequiredExtensions();
		for (auto extensionName : extensions)
			render.addInstanceExtension(extensionName);
		render.addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		// set vertex and fragment shader
		render.setVertexShader("data/shaders/shader.vert.spv");
		render.setFragmentShader("data/shaders/shader.frag.spv");

		// Add a quad to be drawn
		const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
		};
		const std::vector<uint16_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		unsigned int vertexBuffer = render.addBuffer(vertices.size() * sizeof(vertices[0]), vk::BufferUsageFlagBits::eVertexBuffer);
		unsigned int indexBuffer = render.addBuffer(indices.size() * sizeof(indices[0]), vk::BufferUsageFlagBits::eIndexBuffer);

		// Initialise the render with previous info set
		render.init();	// TODO replace with wanted functions calls to initialize the render

		// render have created actual buffers, we can copy data into it
		render.fillBuffer<Vertex>(vertexBuffer, vertices);
		render.fillBuffer<uint16_t>(indexBuffer, indices);

	}

	/** Have to be derivated */
	void quit() {

	}

	/** Have to be derivated */
	void update() {
		window.pollEvents();

	}

	/** Have to be derivated */
	bool draw() {
		render.drawFrame();

		return !window.isClosed();
	}

private:
	RenderWindow window;

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
