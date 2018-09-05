#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>

#include <vulkan/vulkan.hpp>

class Application {
public:
	void run() {
		init();
		mainLoop();
		cleanup();
	}

private:
	void init() {

	}

	void mainLoop() {

	}

	void cleanup() {

	}
};

#endif
