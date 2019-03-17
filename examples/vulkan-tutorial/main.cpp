#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "Application.hpp"
#include "RenderWindow.hpp"

class Tutorial : public Application {
public:

	Tutorial() : Application("Vulkan", VK_MAKE_VERSION(0, 0, 0)),
				 window(800, 600, "VkHelloTriangle") {

	}

	~Tutorial() {

	}

	/** Have to be derivated to render in a Window */
	vk::SurfaceKHR createRenderSurface(vk::Instance instance) {
		return window.createSurface(instance);
	}

	/** Have to be derivated */
	void init() {

		auto extensions = window.getRequiredExtensions();
		for (auto extensionName : extensions)
			render.addInstanceExtension(extensionName);
		render.addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		render.init();	// TODO replace with wanted functions calls to initialize the render

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
