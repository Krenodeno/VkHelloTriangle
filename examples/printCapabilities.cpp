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
		std::cout << "Could not load Vulkan library !";
		return -1;
	}

	if (!loader.LoadExportedEntryPoints()) {
		std::cout << "Could not load exported function !\n";
		return -1;
	}

	if (!loader.LoadGlobalEntryPoints()) {
		std::cout << "Could not load global level functions !\n";
		return -1;
	}

	
	// Get supported extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available Instance extensions:\n";
	int cpt = 0;
	for (const auto& extension : extensions) {
		std::cout << cpt++ << "\t" << extension.extensionName << "\n";
	}

	// Get supported layers
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	std::cout << "Available Instance layers:\n";
	cpt = 0;
	for (const auto& layer : layers) {
		std::cout << cpt++ << "\t" << layer.layerName << "\n";
	}
	

	VkInstance instance = VK_NULL_HANDLE;

	createInstance(instance);

	loader.loadInstanceEntryPoints(instance);

	// Enumerate Physical Devices 

	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	// For each, print informations

	std::cout << "\nVulkan enabled physical devices :\n";
	for (auto physicalDevice : physicalDevices) {
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		VkPhysicalDeviceFeatures features = {};
		vkGetPhysicalDeviceFeatures(physicalDevice, &features);

		std::cout << properties.deviceName << "\n";
	}

	vkDestroyInstance(instance, nullptr);

	return 0;
		
}