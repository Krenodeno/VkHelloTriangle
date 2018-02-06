#include <iostream>
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

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (result != VK_SUCCESS)
		std::cout << "Failed to create instance !\n";
}

int main(int argc, char* argv[]) {
	VulkanLoader loader;

	loader.load();

	if (!loader.isLoaded()) {
		std::cout << "Could not load Vulkan library !\n";
		return -1;
	}

	loader.LoadExportedEntryPoints();

	VkInstance instance = VK_NULL_HANDLE;

	createInstance(instance);

	vkDestroyInstance(instance, nullptr);

	return 0;
		
}