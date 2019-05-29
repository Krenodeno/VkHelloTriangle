#include "Application.hpp"

#include <chrono>

Application::Application(std::string appName, uint32_t appVersion)
		: name(appName), version(appVersion) {

}

Application::~Application() {

}

void Application::run() {

	init();

	do {
		update();
	} while (draw());

	quit();
}
