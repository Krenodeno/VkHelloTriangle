#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>
#include <string>

#include "RenderWindow.hpp"
#include "Render.hpp"

class Application {
public:

	Application();
	~Application();

	void run() {
		init();
		mainLoop();
	}

private:
	void init();
	void initWindow();
	void mainLoop();

	RenderWindow window;
	Render render;
	std::string name;
	int version;		// au format vulkan

};

#endif
