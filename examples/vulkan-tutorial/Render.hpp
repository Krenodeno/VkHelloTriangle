#ifndef RENDER_HPP
#define RENDER_HPP

#include <vector>
#include <iostream>
#include <string>

#include <vulkan/vulkan.hpp>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT,
	VkDebugUtilsMessageTypeFlagsEXT,
	const VkDebugUtilsMessengerCallbackDataEXT*,
	void*);

class Render {
public:
	Render();
	~Render();

	void init();
	void cleanup();

	void addLayer(const char*);
	void addExtension(const char*);

	void enableValidationLayer();

private:

	bool validationLayerEnabled;
	std::vector<const char*> layers;
	std::vector<const char*> extensions;

	vk::DispatchLoaderStatic dispatchLoader;

	vk::Instance instance;

	vk::DebugUtilsMessengerEXT callback;

	// initialising functions

	void createInstance();

	void setupDebugCallback();

	// Tools functions

	bool checkExtensionSupport(const char*);
	bool checkLayerSupport(const char*);
};

#endif
