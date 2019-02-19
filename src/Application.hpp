#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>
#include <string>

#include "RenderWindow.hpp"
#include "Render.hpp"

class Application {
public:

	Application(const int width, const int height, std::string appName, uint32_t version);
	virtual ~Application();

	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	void run();

	vk::SurfaceKHR createRenderSurface(vk::Instance);

	vk::Extent2D windowExtent();

	std::string getName() { return name; }
	uint32_t getVersion() { return version; }

protected:
	void initWindow();

	RenderWindow window;
	Render render;
	std::string name;
	uint32_t version;	// vulkan style : VK_MAKE_VERSION(major, minor, patch)

};

vk::SurfaceKHR createSurface(Application*, vk::Instance);

#endif
