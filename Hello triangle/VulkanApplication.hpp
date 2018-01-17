#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanRenderer.hpp"

void onWindowResized(GLFWwindow* window, int width, int height);

class VulkanApplication
{
public:

	VulkanApplication();

	void run();

private:

	void mainLoop();

	GLFWwindow * window;

	int width, height;

	VulkanRenderer renderer;

	void initWindow();

	void createRendererSurface();

};

