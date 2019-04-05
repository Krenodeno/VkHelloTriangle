#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>
#include <string>

#include "Render.hpp"

class Application {
public:

	Application(std::string appName, uint32_t version);
	virtual ~Application();

	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	/** Should return true to continue, and false to end the main loop */
	virtual bool draw() = 0;

	void run();

	std::string getName() { return name; }
	uint32_t getVersion() { return version; }

protected:

	Render render;
	std::string name;
	uint32_t version;	// vulkan style : VK_MAKE_VERSION(major, minor, patch)

};

#endif
