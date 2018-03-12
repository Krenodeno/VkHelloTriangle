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

	void createRendererSurface(VkInstance instance, VkSurfaceKHR* surface);

private:

	void mainLoop();

	GLFWwindow * window;

	int width, height;

	VulkanRenderer renderer;

	void initWindow();

	std::vector<const char*> getGLFWRequiredExtensions();

};

void createSurface(VulkanApplication* app, VkInstance instance, VkSurfaceKHR* surface);

