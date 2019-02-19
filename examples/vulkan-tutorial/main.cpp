#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <chrono>

#include "Application.hpp"

class Tutorial : public Application {
public:

	Tutorial() : Application(800, 600, "Vulkan", VK_MAKE_VERSION(0, 0, 0)) {

	}

	~Tutorial() {

	}

	void init() {

		auto start = std::chrono::steady_clock::now();

		auto extensions = window.getRequiredExtensions();
		for (auto extensionName : extensions)
			render.addInstanceExtension(extensionName);
		render.addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		#if defined(DEBUG)
		render.enableValidationLayer();
		#endif

		render.init();

		auto end = std::chrono::steady_clock::now();

		auto elapsedTime = end - start;

		std::cout << "Vulkan Initialisation took " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count() << "ms\n";
	}

	void quit() {

	}

	void update() {

	}

	void draw() {
		render.drawFrame();
	}

private:

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
