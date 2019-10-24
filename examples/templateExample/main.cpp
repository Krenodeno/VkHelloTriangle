#include "WindowedApp.hpp"
#include "Render.hpp"

class ShadowMapExample : public WindowedApp<Render> {
public:

	ShadowMapExample() : WindowedApp("shadowMap", VK_MAKE_VERSION(0, 0, 0)) {
		window.setKeyCallback(
			[&](Key key, int scancode, int action, KeyModifiersFlags modifiers) {
				if (key == Key::eEscape && action == GLFW_PRESS) {
					window.close();
				}
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

		// initialise render
		render.init();
	}

	/** Called once at the program exit to handle resource destruction */
	void quit() {

	}

	/** Called at each frame before draw */
	void update() {

	}

	/** Return true to continue, false to end the main loop and exit the program */
	bool draw() {
		return false;
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