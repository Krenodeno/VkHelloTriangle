#include "VulkanApplication.hpp"

VulkanApplication::VulkanApplication() : width(800), height(600)
{
}

void VulkanApplication::run()
{
	initWindow();
	renderer.init(window);
	mainLoop();
}

void VulkanApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		renderer.updateUniformBuffer();
		renderer.drawFrame();
	}

	renderer.waitIdle();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void VulkanApplication::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);

	glfwSetWindowUserPointer(window, this);
	//glfwSetWindowSizeCallback(window, VulkanApplication::onWindowResized);
}

void VulkanApplication::onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;

	VulkanApplication* app = reinterpret_cast<VulkanApplication*>(glfwGetWindowUserPointer(window));
	// TODO : 
	//app->recreateSwapChain();
}
