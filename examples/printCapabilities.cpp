#include <iostream>
#include <stdexcept>
#include <vector>
#include "VulkanLoader.hpp"

void createInstance(VkInstance& instance) {

	// Construct Vulkan structs needed to create instance
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance !");
}

int main(int argc, char* argv[]) {
	VulkanLoader loader;

	loader.load();

	if (!loader.isLoaded()) {
		throw std::runtime_error("Could not load Vulkan library !");
		return -1;
	}

	if (!loader.LoadExportedEntryPoints())
		throw std::runtime_error("Could not load exported function !");

	if (!loader.LoadGlobalEntryPoints())
		throw std::runtime_error("Could not load global level functions !");


	// Get supported extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available extensions:" << std::endl;
	int cpt = 0;
	for (const auto& extension : extensions) {
		std::cout << cpt++ << "\t" << extension.extensionName << std::endl;
	}

	// Get supported layers
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	std::cout << "Available layers:" << std::endl;
	cpt = 0;
	for (const auto& layer : layers) {
		std::cout << cpt++ << "\t" << layer.layerName << std::endl;
	}


	VkInstance instance = VK_NULL_HANDLE;

	//createInstance(instance);

	//loader.loadInstanceEntryPoints(instance);

	//vkDestroyInstance(instance, nullptr);

	return 0;
		
}