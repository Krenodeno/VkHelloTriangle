#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>

#include <vulkan/vulkan.hpp>

#include "RenderWindow.hpp"

class Application {
public:

	Application();
	~Application();

	void run() {
		init();
		mainLoop();
		cleanup();
	}

private:
	void init();
	void initWindow();
	void mainLoop();
	void cleanup();

	RenderWindow window;
};

#endif
