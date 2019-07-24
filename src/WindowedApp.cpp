#include "WindowedApp.hpp"

#include "chrono"

WindowedApp::WindowedApp(std::string appName, uint32_t version, int width, int height)
		: Application(appName, version),
		  window(width, height, appName) {
	render.setAppName(appName);
	render.setAppVersion(version);
	render.setSurfaceCreationFunction(
		[&](vk::Instance instance, vk::DispatchLoaderDynamic d) {
			return window.createSurface(instance, d);
		}
	);
}

WindowedApp::~WindowedApp() {

}

void WindowedApp::run() {
#ifdef DEBUG
	auto start = std::chrono::high_resolution_clock::now();
	render.enableValidationLayer();
#endif

	init();

#ifdef DEBUG
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsedTime = end - start;
	std::cout << "Vulkan Initialisation took ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
	std::cout << "ms\n";
#endif
	do {
		render.waitForFences();
		update();
	} while (draw());

	std::cout << std::endl;

	render.waitDeviceIdle();

	quit();
}