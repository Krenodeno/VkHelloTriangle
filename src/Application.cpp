#include "Application.hpp"

#include <chrono>

Application::Application(std::string appName, uint32_t appVersion)
		: name(appName), version(appVersion) {
	render.setParentApplication(this);
	render.setSurfaceCreationFunction(createSurface);
	render.setAppName(appName);
	render.setAppVersion(appVersion);
}

Application::~Application() {

}

void Application::run() {
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
		update();
	} while (draw());

	render.waitDeviceIdle();

	quit();
}

vk::SurfaceKHR createSurface(Application* app, vk::Instance instance) {
	return app->createRenderSurface(instance);
}
