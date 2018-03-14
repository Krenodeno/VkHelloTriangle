#include <string>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "VulkanLoader.hpp"

using std::cout;

void printVulkanVersion() {
	cout << "===========\n";
	cout << "VULKAN INFO\n";
	cout << "===========\n";

	cout << "\nVulkan API Version: " << VK_VERSION_MAJOR(VK_API_VERSION_1_0) << "." << VK_VERSION_MINOR(VK_API_VERSION_1_0) << "." << VK_HEADER_VERSION << "\n";
	cout << "\n\n";
}

/**
 * get available extensions
 */
std::vector<VkExtensionProperties> getInstanceExtensions(const char* layerName) {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, extensions.data());

	return extensions;
}

void printExtensionsProperties(std::vector<VkExtensionProperties> extensions) {
	cout << "Instance Extensions:\n";
	cout << "====================\n";
	cout << "Instance Extensions\tcount = " << extensions.size() << "\n";
	for (const auto& extension : extensions) {
		std::string extName = extension.extensionName;
		cout << "\t" << extName << std::setw(64 - extName.size()) << ": exension revision " << extension.specVersion << "\n";
	}
	cout << "\n\n";
}

/**
* get available layers
*/
std::vector<VkLayerProperties> getInstanceLayers() {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

	return layers;
}

void printLayersProperties(std::vector<VkLayerProperties> layers, std::vector<VkPhysicalDevice> physicalDevices) {
	cout << "Layers: count = " << layers.size() << "\n";
	cout << "=======\n";
	for (const auto& layer : layers) {
		cout << layer.layerName << " (" << layer.description << ") ";
		cout << "Vulkan version " << VK_VERSION_MAJOR(layer.specVersion) << "." << VK_VERSION_MINOR(layer.specVersion) << "." << VK_VERSION_PATCH(layer.specVersion);
		cout << ", layer version " << layer.implementationVersion << "\n";
		
		std::vector<VkExtensionProperties> layerExtensions = getInstanceExtensions(layer.layerName);
		cout << "\tLayer Extensions\tcount = " << layerExtensions.size() << "\n";
		for (auto extension : layerExtensions) {
			std::string extName = extension.extensionName;
			cout << "\t\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
		}

		// Devices
		unsigned int deviceId = 0;
		cout << "\tDevices\tcount = " << physicalDevices.size() << "\n";
		for (auto device : physicalDevices) {
			VkPhysicalDeviceProperties properties = {};
			vkGetPhysicalDeviceProperties(device, &properties);
			cout << "\t\tGPU id\t: " << deviceId++ << " (" << properties.deviceName << ")\n";

			uint32_t extensionCount = 0;
			vkEnumerateDeviceExtensionProperties(device, layer.layerName, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, layer.layerName, &extensionCount, extensions.data());

			cout << "\t\tLayer-Device Extensions count = " << extensionCount << "\n";
			for (auto extension : extensions) {
				std::string extName = extension.extensionName;
				cout << "\t\t\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
			}
		}

		cout << "\n";
	}
}

std::vector<VkLayerProperties> getDeviceLayers(VkPhysicalDevice physicalDevice) {
	uint32_t layerCount = 0;
	vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, nullptr);

	std::vector<VkLayerProperties> layers(layerCount);
	vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, layers.data());

	return layers;
}

std::vector<VkExtensionProperties> getDeviceExtenstions(VkPhysicalDevice physicalDevice, const char* layerName) {
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(physicalDevice, layerName, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, layerName, &extensionCount, extensions.data());

	return extensions;
}

void createInstance(VkInstance& instance, const std::vector<char*>& layerNames, const std::vector<char*>& extensionNames) {

	// Construct Vulkan structs needed to create instance
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "vulkaninfo";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	// Enable layers
	createInfo.enabledLayerCount = layerNames.size();
	createInfo.ppEnabledLayerNames = layerNames.data();
	// Enable extensions
	createInfo.enabledExtensionCount = extensionNames.size();
	createInfo.ppEnabledExtensionNames = extensionNames.data();


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

	// Load vulkan dynamic lib and then load all functions needed for instance query and creation

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

	// Print header version
	printVulkanVersion();
	
	// Query Instance about available extensions and print them
	std::vector<VkExtensionProperties> extensions = getInstanceExtensions(nullptr);
	printExtensionsProperties(extensions);

	std::vector<char*> extensionNames(extensions.size());
	for (unsigned int i = 0; i < extensions.size(); ++i) {
		extensionNames[i] = extensions[i].extensionName;
	}

	// Query Instance about available layers
	std::vector<VkLayerProperties> layers = getInstanceLayers();

	std::vector<char*> layerNames(layers.size());
	for (unsigned int i = 0; i < layers.size(); ++i) {
		layerNames[i] = layers[i].layerName;
	}
	
	// Validation layer from LUNARG
	std::vector<char*> validationLayer = { "VK_LAYER_LUNARG_standard_validation" };

	// Create Instance with validation layer and all available extensions
	VkInstance instance = VK_NULL_HANDLE;
	createInstance(instance, validationLayer, extensionNames);

	// Load functions only available with an active Instance
	if (!loader.loadInstanceEntryPoints(instance)) {
		std::cerr << "Could not load instance level functions !\n";
		vkDestroyInstance(instance, nullptr);
		return -1;
	}

	// Enumerate Physical Devices 
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

	// Print Instance Layers properties and associated Device extensions
	printLayersProperties(layers , physicalDevices);

	int cpt = 0;
	cout << "\nVulkan enabled physical devices :\n";
	for (auto physicalDevice : physicalDevices) {
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		VkPhysicalDeviceFeatures features = {};
		vkGetPhysicalDeviceFeatures(physicalDevice, &features);

		uint32_t queueFamilyPropertiesCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, queueFamilyProperties.data());

		cout << cpt << "\t" << properties.deviceName << "\n";
		cout << "\tQueue family properties count : " << queueFamilyPropertiesCount << "\n";
		int cpt2 = 0;
		for (auto queueFamilyProperty : queueFamilyProperties) {
			cout << "\t" << cpt2++ << "\tQueue count : " << queueFamilyProperty.queueCount << "\n\t\tQueue flags :";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				cout << " GRAPHICS ";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT)
				cout << " COMPUTE ";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT)
				cout << " TRANSFER ";
			if (queueFamilyProperty.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
				cout << " SPARSE ";
			cout << "\n";
		}
	}

	// Take first physical device for device creation
	
	
	// End of the program, destroy Vulkan objects
	vkDestroyInstance(instance, nullptr);

	return 0;
		
}