#include "Application.hpp"

Application::Application() : window(800, 600, "Vulkan"){

}

Application::~Application() {

}

void Application::init() {

}

void Application::mainLoop() {
	while(!window.isClosed()) {
		window.pollEvents();
	}
}

void Application::cleanup() {

}
