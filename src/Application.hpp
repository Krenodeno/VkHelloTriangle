#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>
#include <string>

#include "RenderWindow.hpp"
#include "Render.hpp"

class Application {
public:

	Application(const int width, const int height, std::string appName);
	virtual ~Application();

	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	void run();

	vk::SurfaceKHR createRenderSurface(vk::Instance);

	vk::Extent2D windowExtent();

protected:
	void initWindow();

	RenderWindow window;
	Render render;
	std::string name;
	int version;		// vulkan style

};

vk::SurfaceKHR createSurface(Application*, vk::Instance);

#endif
