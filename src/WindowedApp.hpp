#ifndef WINDOWED_APP_HPP
#define WINDOWED_APP_HPP

#include "Application.hpp"

#include "RenderWindow.hpp"

#include "chrono"

template<typename Renderer>
class WindowedApp : public Application {

public:
	WindowedApp(std::string appName, uint32_t version, int width = 800, int height = 600)
		: Application(appName, version), window(width, height, appName)
	{
		render.setAppName(appName);
		render.setAppVersion(version);
		render.setSurfaceCreationFunction(
			[&](vk::Instance instance, vk::DispatchLoaderDynamic d) {
				return window.createSurface(instance, d);
			}
		);
	}
	virtual ~WindowedApp() {}

	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	/** Should return true to continue, and false to end the main loop */
	virtual bool draw() = 0;

	void run() {
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
			window.pollEvents();
			update();
		} while (draw());

		std::cout << "Quitting..." << std::endl;

		render.waitDeviceIdle();

		quit();
	}

protected:

	RenderWindow window;
	Renderer render;

};

#endif