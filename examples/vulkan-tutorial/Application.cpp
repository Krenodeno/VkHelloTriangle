#include "Application.hpp"

Application::Application() : window(800, 600, "Vulkan"){

}

Application::~Application() {

}

vk::SurfaceKHR Application::createRenderSurface(vk::Instance instance) {
	return window.createSurface(instance);
}

void Application::init() {
	auto extensions = window.getRequiredExtensions();
	for (auto extensionName : extensions)
		render.addExtension(extensionName);
	#if defined(DEBUG)
	render.enableValidationLayer();
	#endif

	render.setParentApplication(this);
	render.setSurfaceCreationFunction(createSurface);

	render.init();
}

void Application::mainLoop() {
	while(!window.isClosed()) {
		window.pollEvents();
	}
}

vk::SurfaceKHR createSurface(Application* app, vk::Instance instance) {
	return app->createRenderSurface(instance);
}
