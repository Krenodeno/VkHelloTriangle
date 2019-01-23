#include "Application.hpp"

Application::Application(const int width, const int height, std::string appName)
	: window(width, height, appName), name(appName) {

}

Application::~Application() {

}

void Application::run() {
	init();
	while (!window.isClosed()) {
		update();
		draw();
		window.pollEvents();
	}

	render.waitDeviceIdle();

	quit();
}

vk::SurfaceKHR Application::createRenderSurface(vk::Instance instance) {
	return window.createSurface(instance);
}

vk::Extent2D Application::windowExtent() {
	return {static_cast<uint32_t>(window.getWidth()), static_cast<uint32_t>(window.getHeight())};
}

vk::SurfaceKHR createSurface(Application* app, vk::Instance instance) {
	return app->createRenderSurface(instance);
}
