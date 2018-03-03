#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "VulkanLoader.hpp"

void printVulkanVersion() {
	std::cout << "===========\n";
	std::cout << "VULKAN INFO\n";
	std::cout << "===========\n";

	std::cout << "\nVulkan API Version: " << VK_VERSION_MAJOR(VK_API_VERSION_1_0) << "." << VK_VERSION_MINOR(VK_API_VERSION_1_0) << "." << VK_HEADER_VERSION << "\n";
	std::cout << "\n\n";
}

void printInstanceExtensions() {
	// Get supported extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Instance Extensions:\n";
	std::cout << "====================\n";
	std::cout << "Instance Extensions\tcount = " << extensionCount << "\n";
	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::setw(32) << ": exension revision " << extension.specVersion << "\n";
	}
	std::cout << "\n\n";
}

void printInstanceLayers() {
	// Get supported layers
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	std::cout << "Layers: count = " << layerCount << "\n";
	std::cout << "=======\n";
	for (const auto& layer : layers) {
		std::cout << layer.layerName << " (" << layer.description << ") ";
		std::cout << "Vulkan version " << VK_VERSION_MAJOR(layer.specVersion) << "." << VK_VERSION_MINOR(layer.specVersion) << "." << VK_VERSION_PATCH(layer.specVersion);
		std::cout << ", layer version " << layer.implementationVersion << "\n\n";
		
	}
}

void createInstance(VkInstance& instance, const std::vector<char*>& layerNames) {

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
	createInfo.enabledLayerCount = layerNames.size();
	createInfo.ppEnabledLayerNames = layerNames.data();

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (result != VK_SUCCESS)
		throw std::runtime_error("Failed to create instance !");
}


void createDevice(VkPhysicalDevice& physicalDevice, VkDevice& device) {
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	//vkGetPhysicalDeviceQueueFamilyProperties();
}

int main(int argc, char* argv[]) {
	VulkanLoader loader;

	loader.load();

	if (!loader.isLoaded()) {
		std::cerr << "Could not load Vulkan library !";
		return -1;
	}

	if (!loader.LoadExportedEntryPoints()) {
		std::cerr << "Could not load exported function !\n";
		return -1;
	}

	if (!loader.LoadGlobalEntryPoints()) {
		std::cerr << "Could not load global level functions !\n";
		return -1;
	}

	printVulkanVersion();
	
	printInstanceExtensions();

	printInstanceLayers();
	
	std::vector<char*> validationLayer = { "VK_LAYER_LUNARG_standard_validation" };

	VkInstance instance = VK_NULL_HANDLE;

	createInstance(instance, validationLayer);

	if (!loader.loadInstanceEntryPoints(instance)) {
		std::cerr << "Could not load instance level functions !\n";
		return -1;
	}

	// Enumerate Physical Devices 

	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	// For each, print informations

	int cpt = 0;
	std::cout << "\nVulkan enabled physical devices :\n";
	for (auto physicalDevice : physicalDevices) {
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		VkPhysicalDeviceFeatures features = {};
		vkGetPhysicalDeviceFeatures(physicalDevice, &features);

		uint32_t queueFamilyPropertiesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());

		std::cout << cpt << "\t" << properties.deviceName << "\n";
		std::cout << "\tQueue family properties count : " << queueFamilyPropertiesCount << "\n";
		int cpt2 = 0;
		for (auto queueFamilyProperty : queueFamilyProperties) {
			std::cout << "\t" << cpt2++ << "\tQueue count : " << queueFamilyProperty.queueCount << "\n\t\tQueue flags :";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				std::cout << " GRAPHICS ";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT)
				std::cout << " COMPUTE ";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT)
				std::cout << " TRANSFER ";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
				std::cout << " SPARSE ";
			std::cout << "\n";
		}
	}

	// Take first physical device for device creation
	
	

	vkDestroyInstance(instance, nullptr);

	return 0;
		
}