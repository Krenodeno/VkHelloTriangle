#include "VulkanApplication.hpp"

VulkanApplication::VulkanApplication() : width(800), height(600)
{
}

void VulkanApplication::run()
{
	initWindow();
	//createRendererSurface();
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

	glfwSetWindowUserPointer(window, &renderer);
	glfwSetWindowSizeCallback(window, onWindowResized);
}

void VulkanApplication::createRendererSurface()
{
	auto instance = renderer.getInstance();
	auto surface = renderer.getSurface();

	if (surface != VK_NULL_HANDLE) {
		vkDestroySurfaceKHR(instance, surface, nullptr);
		surface = VK_NULL_HANDLE;
	}

	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface !");
}

void onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;

	VulkanRenderer* render = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
	render->recreateSwapChain();
}
