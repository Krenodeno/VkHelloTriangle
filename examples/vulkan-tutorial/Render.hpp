#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <iostream>
#include <string>
#include <functional>

#include <vulkan/vulkan.hpp>

/** Callback */

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT,
	VkDebugUtilsMessageTypeFlagsEXT,
	const VkDebugUtilsMessengerCallbackDataEXT*,
	void*);

/** struct */

struct QueueFamilyIndices {
	int graphicsFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0;
	}
};

/* forward declaration */
class Application;

/* used as callback function to delegate surface creation  */
using createSurfaceFoncter = std::function<vk::SurfaceKHR(Application*, vk::Instance)>;

class Render {
public:
	Render();
	~Render();

	void init();
	void cleanup();

	void addLayer(const char*);
	void addExtension(const char*);

	void enableValidationLayer();

	void setParentApplication(Application*);

	void setSurfaceCreationFunction(createSurfaceFoncter);

private:

	bool validationLayerEnabled;
	std::vector<const char*> layers;
	std::vector<const char*> extensions;

	Application* parentApp;

	createSurfaceFoncter surfaceCreation;

	vk::DispatchLoaderStatic dispatchLoader;

	vk::Instance instance;

	vk::DebugUtilsMessengerEXT callback;

	vk::PhysicalDevice physicalDevice;

	vk::Device device;

	vk::Queue graphicsQueue;

	vk::SurfaceKHR surface;

	// initialising functions

	void createInstance();

	void setupDebugCallback();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSurface();

	// Tools functions

	bool checkExtensionSupport(const char*);
	bool checkLayerSupport(const char*);

	bool isDeviceSuitable(vk::PhysicalDevice);

	QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice);
};

#endif
