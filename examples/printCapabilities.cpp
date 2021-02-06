#include <string>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>

#if defined(USE_WINDOWS_OPERATING_SYSTEM)
# define VK_USE_PLATFORM_WIN32_KHR
#elif defined(USE_LINUX_OPERATING_SYSTEM)
# define VK_USE_PLATFORM_WAYLAND_KHR
# define VK_USE_PLATFORM_XCB_KHR
# define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.hpp>

#if VK_HEADER_VERSION > 121
# define VK_VALIDATION_LAYERS_STRING "VK_LAYER_KHRONOS_validation"
#else
# define VK_VALIDATION_LAYERS_STRING "VK_LAYER_LUNARG_standard_validation"
#endif

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
		auto extName = static_cast<std::string>(extension.extensionName);
		cout << "\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
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

		auto layerExtensions = vk::enumerateInstanceExtensionProperties(static_cast<std::string>(layer.layerName));
		cout << "\tLayer Extensions\tcount = " << layerExtensions.size() << "\n";
		for (auto extension : layerExtensions) {
			auto extName = static_cast<std::string>(extension.extensionName);
			cout << "\t\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
		}

		// Devices
		unsigned int deviceId = 0;
		cout << "\tDevices\tcount = " << physicalDevices.size() << "\n";
		for (auto device : physicalDevices) {
			auto properties = device.getProperties();
			cout << "\t\tGPU id\t: " << deviceId++ << " (" << properties.deviceName << ")\n";

			auto extensions = device.enumerateDeviceExtensionProperties(static_cast<std::string>(layer.layerName));

			cout << "\t\tLayer-Device Extensions count = " << extensions.size() << "\n";
			for (auto extension : extensions) {
				std::string extName = static_cast<std::string>(extension.extensionName);
				cout << "\t\t\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
			}
		}

		cout << "\n";
	}
}

void printQueueFamilies(vk::PhysicalDevice physicalDevice) {
	vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

	uint32_t queueFamilyPropertiesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);

	auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

	cout << "\t" << properties.deviceName << "\n";
	cout << "\tQueue family properties count : " << queueFamilyPropertiesCount << "\n";
	int cpt2 = 0;
	for (auto queueFamilyProperty : queueFamilyProperties) {
		cout << "\t" << cpt2++ << "\tQueue count : " << queueFamilyProperty.queueCount << "\n\t\tQueue flags :";
		if (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics)
			cout << " GRAPHICS ";
		if (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eCompute)
			cout << " COMPUTE ";
		if (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eTransfer)
			cout << " TRANSFER ";
		if (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eSparseBinding)
			cout << " SPARSE ";
		cout << "\n";
	}
}

void printWaylandSurfaceCapabilities(vk::Instance instance, vk::PhysicalDevice physicalDevice) {
	// Connect to wayland Display
	wl_display* display = wl_display_connect(NULL);

	if (display == NULL) {
		return;
	}

	// Get registry
	wl_registry* registry = wl_display_get_registry(display);

	wl_registry_listener registry_listener = {
		[](void* data, wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
			std::cout << "Got a registry event for " << interface << " id " << id << "\n";
			if (strcmp(interface, "wl_compositor") == 0)
				data = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
		},
		[](void* data, wl_registry* registry, uint32_t id) {}
	};
	wl_compositor* compositor;
	wl_registry_add_listener(registry, &registry_listener, compositor);

	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	auto presentationSupport = physicalDevice.getWaylandPresentationSupportKHR(0, display);

	vk::WaylandSurfaceCreateInfoKHR waylandSurfaceCreateInfo;
	waylandSurfaceCreateInfo.display = display;
	//waylandSurfaceCreateInfo.surface = ;
	//auto waylandSurface = instance.createWaylandSurfaceKHR(waylandSurfaceCreateInfo);

	// Destroy surface
	//instance.destroySurfaceKHR(waylandSurface);

	// Disconnect wayland display
	wl_display_disconnect(display);
}

void printSurfaceCapabilities(vk::Instance instance, vk::PhysicalDevice physicalDevice) {
	// Linux surfaces

	// Wayland
	printWaylandSurfaceCapabilities(instance, physicalDevice);
}

std::vector<vk::LayerProperties> getDeviceLayers(vk::PhysicalDevice physicalDevice) {
	auto layers = physicalDevice.enumerateDeviceLayerProperties();

	return layers;
}

std::vector<vk::ExtensionProperties> getDeviceExtensions(vk::PhysicalDevice physicalDevice, const std::string layerName) {
	auto extensions = physicalDevice.enumerateDeviceExtensionProperties(layerName);

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


vk::Device createDevice(vk::PhysicalDevice physicalDevice) {

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

	// Validation layer
	std::vector<const char*> validationLayers = { VK_VALIDATION_LAYERS_STRING };

	// Create Instance with validation layer and all available extensions
	auto instance = createInstance(validationLayers, extensionNames);

	// Enumerate Physical Devices
	auto physicalDevices = instance.enumeratePhysicalDevices();

	// Print Instance Layers properties and associated Device extensions
	printLayersProperties(layers , physicalDevices);

	cout << "\nVulkan enabled physical devices :\n";
	for (auto physicalDevice : physicalDevices) {
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(physicalDevice, &properties);

		VkPhysicalDeviceFeatures features = {};
		vkGetPhysicalDeviceFeatures(physicalDevice, &features);

		printQueueFamilies(physicalDevice);

		// Surface capabilities
		printSurfaceCapabilities(instance, physicalDevice);
	}

	// Take first physical device for device creation


	// End of the program, destroy Vulkan objects
	instance.destroy();

	return 0;

}
