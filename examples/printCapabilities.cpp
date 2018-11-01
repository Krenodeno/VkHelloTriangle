#include <string>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

using std::cout;

void printVulkanVersion() {
	cout << "===========\n";
	cout << "VULKAN INFO\n";
	cout << "===========\n";

	auto version = vk::enumerateInstanceVersion();

	cout << "\nVulkan API Version: " << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version) << "\n";
	cout << "\n\n";
}

void printExtensionsProperties(std::vector<vk::ExtensionProperties> extensions) {
	cout << "Instance Extensions:\n";
	cout << "====================\n";
	cout << "Instance Extensions\tcount = " << extensions.size() << "\n";
	for (const auto& extension : extensions) {
		std::string extName = extension.extensionName;
		cout << "\t" << extName << std::setw(64 - extName.size()) << ": exension revision " << extension.specVersion << "\n";
	}
	cout << "\n\n";
}

void printLayersProperties(std::vector<vk::LayerProperties> layers, std::vector<vk::PhysicalDevice> physicalDevices) {
	cout << "Layers: count = " << layers.size() << "\n";
	cout << "=======\n";
	for (const auto& layer : layers) {
		cout << layer.layerName << " (" << layer.description << ") ";
		cout << "Vulkan version " << VK_VERSION_MAJOR(layer.specVersion) << "." << VK_VERSION_MINOR(layer.specVersion) << "." << VK_VERSION_PATCH(layer.specVersion);
		cout << ", layer version " << layer.implementationVersion << "\n";

		auto layerExtensions = vk::enumerateInstanceExtensionProperties(std::string(layer.layerName));
		cout << "\tLayer Extensions\tcount = " << layerExtensions.size() << "\n";
		for (auto extension : layerExtensions) {
			std::string extName = extension.extensionName;
			cout << "\t\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
		}

		// Devices
		unsigned int deviceId = 0;
		cout << "\tDevices\tcount = " << physicalDevices.size() << "\n";
		for (auto device : physicalDevices) {
			auto properties = device.getProperties();
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

vk::Instance createInstance(const std::vector<const char*>& layerNames, const std::vector<char*>& extensionNames) {

	// Construct Vulkan structs needed to create instance
	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = "vulkaninfo";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo createInfo;
	createInfo.pApplicationInfo = &appInfo;
	// Enable layers
	createInfo.enabledLayerCount = layerNames.size();
	createInfo.ppEnabledLayerNames = layerNames.data();
	// Enable extensions
	createInfo.enabledExtensionCount = extensionNames.size();
	createInfo.ppEnabledExtensionNames = extensionNames.data();


	return vk::createInstance(createInfo);
}


vk::Device createDevice(VkPhysicalDevice& physicalDevice) {

}

int main(int argc, char* argv[]) {

	// Print header version
	printVulkanVersion();

	// Query Instance about available extensions and print them
	auto extensions = vk::enumerateInstanceExtensionProperties();
	printExtensionsProperties(extensions);

	std::vector<char*> extensionNames(extensions.size());
	for (unsigned int i = 0; i < extensions.size(); ++i) {
		extensionNames[i] = extensions[i].extensionName;
	}

	// Query Instance about available layers
	auto layers = vk::enumerateInstanceLayerProperties();

	std::vector<char*> layerNames(layers.size());
	for (unsigned int i = 0; i < layers.size(); ++i) {
		layerNames[i] = layers[i].layerName;
	}

	// Validation layer from LUNARG
	std::vector<const char*> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

	// Create Instance with validation layer and all available extensions
	auto instance = createInstance(validationLayers, extensionNames);

	// Enumerate Physical Devices
	auto physicalDevices = instance.enumeratePhysicalDevices();

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
	instance.destroy();

	return 0;

}
