#include "VulkanApplication.hpp"

VulkanApplication::VulkanApplication() : width(800), height(600)
{
}

void VulkanApplication::run()
{
	initWindow();
	renderer.addExtensions(getGLFWRequiredExtensions());
	renderer.setCreateSurfaceFunction(createSurface);
	renderer.setParentApplication(this);
	renderer.setWidth(width);
	renderer.setHeight(height);
	renderer.init();
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

void VulkanApplication::createRendererSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		throw std::runtime_error("Failed to create window surface !");
}

std::vector<const char*> VulkanApplication::getGLFWRequiredExtensions()
{
	// Get GLFW extensions
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensionNames(glfwExtensionCount);
	for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		extensionNames[i] = glfwExtensions[i];

	return extensionNames;
}

void onWindowResized(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) return;

	VulkanRenderer* render = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
	render->setWidth(width);
	render->setHeight(height);
	render->recreateSwapChain();
}

void createSurface(VulkanApplication* app, VkInstance instance, VkSurfaceKHR * surface)
{
	app->createRendererSurface(instance, surface);
}
