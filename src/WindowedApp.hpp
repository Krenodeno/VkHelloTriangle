#ifndef WINDOWED_APP_HPP
#define WINDOWED_APP_HPP

#include "Application.hpp"

#include "RenderWindow.hpp"
#include "Render.hpp"

class WindowedApp : public Application {

public:
	WindowedApp(std::string appName, uint32_t version, int width = 800, int height = 600);
	virtual ~WindowedApp();

	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	/** Should return true to continue, and false to end the main loop */
	virtual bool draw() = 0;

	void run();

protected:

	RenderWindow window;
	Render render;

};

#endif