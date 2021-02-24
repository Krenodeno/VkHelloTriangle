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
	cout << "==========\n";
	cout << "VULKANINFO\n";
	cout << "==========\n";

	auto version = vk::enumerateInstanceVersion();

	cout << "\nVulkan Instance Version: " << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version) << "\n";
	cout << "\n\n";
}

void printExtensionsProperties(std::vector<vk::ExtensionProperties> extensions) {
	// header
	std::ostringstream countString;
	countString << "Instance Extensions: count = " << extensions.size() << "\n";
	cout << countString.str();
	const int stringSize = countString.str().size() - 1;
	for(int i = 0; i < stringSize; i++)
		cout << "=";
	cout << "\n";

	for (const auto& extension : extensions) {
		auto extName = static_cast<std::string>(extension.extensionName);
		cout << "\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
	}
	cout << "\n\n";
}

void printLayersProperties(std::vector<vk::LayerProperties> layers, std::vector<vk::PhysicalDevice> physicalDevices) {
	// header
	std::ostringstream countString;
	countString << "Layers: count = " << layers.size() << "\n";
	const int stringSize= countString.str().size() - 1;
	for (int i = 0; i < stringSize; i++)
		cout << "=";
	cout << "\n";

	for (const auto& layer : layers) {
		cout << layer.layerName << " (" << layer.description << ") ";
		cout << "Vulkan version " << VK_VERSION_MAJOR(layer.specVersion) << "." << VK_VERSION_MINOR(layer.specVersion) << "." << VK_VERSION_PATCH(layer.specVersion);
		cout << ", layer version " << layer.implementationVersion << ":\n";

		auto layerExtensions = vk::enumerateInstanceExtensionProperties(static_cast<std::string>(layer.layerName));
		cout << "\tLayer Extensions\tcount = " << layerExtensions.size() << "\n";
		for (auto extension : layerExtensions) {
			auto extName = static_cast<std::string>(extension.extensionName);
			cout << "\t\t" << extName << std::setw(64 - extName.size()) << ": extension revision " << extension.specVersion << "\n";
		}

		// Devices
		unsigned int deviceId = 0;
		cout << "\tDevices: count = " << physicalDevices.size() << "\n";
		for (auto device : physicalDevices) {
			auto properties = device.getProperties();
			cout << "\t\tGPU id = " << deviceId++ << " (" << properties.deviceName << ")\n";

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

/*** SURFACE ***/

void printSurfaceCapabilities(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {

	auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

	auto surfaceSupport = physicalDevice.getSurfaceSupportKHR(0, surface);

	auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);

	auto surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);

	cout << "\tFormats: count = " << surfaceFormats.size() << "\n";
	for (int i = 0; i < surfaceFormats.size(); i++) {
		cout << "\t\tSurfaceFormat[" << i << "]:\n";
		cout << "\t\t\tformat = " << vk::to_string(surfaceFormats[i].format) << "\n";
		cout << "\t\t\tcolorSpace = " << vk::to_string(surfaceFormats[i].colorSpace) << "\n";
	}

	cout << "\tPresent Modes: count = " << surfacePresentModes.size() << "\n";
	for (auto mode : surfacePresentModes) {
		cout << "\t\t" << vk::to_string(mode) << "\n";
	}

	cout << "\tVkSurfaceCapabilitiesKHR:\n";
	cout << "\t-------------------------\n";

	cout << "\t\tminImageCount       = " << surfaceCapabilities.minImageCount << "\n";
	cout << "\t\tmaxImageCount       = " << surfaceCapabilities.maxImageCount << "\n";

	cout << "\t\tcurrentExtent:\n";
	cout << "\t\t\twidth  = " << surfaceCapabilities.currentExtent.width << "\n";
	cout << "\t\t\theight = " << surfaceCapabilities.currentExtent.height << "\n";

	cout << "\t\tminImageExtent:\n";
	cout << "\t\t\twidth  = " << surfaceCapabilities.minImageExtent.width << "\n";
	cout << "\t\t\theight = " << surfaceCapabilities.minImageExtent.height << "\n";

	cout << "\t\tmaxImageExtent:\n";
	cout << "\t\t\twidth  = " << surfaceCapabilities.maxImageExtent.width << "\n";
	cout << "\t\t\theight = " << surfaceCapabilities.maxImageExtent.height << "\n";

	cout << "\t\tmaxImageArrayLayers = " << surfaceCapabilities.maxImageArrayLayers << "\n";

	cout << "\t\tsupportedTransforms: count = \n";
	cout << "\t\t\t" << vk::to_string(surfaceCapabilities.supportedTransforms) << "\n";

	cout << "\t\tcurrentTransform    = " << vk::to_string(surfaceCapabilities.currentTransform) << "\n";

	cout << "\t\tsupportedCompositeAlpha : count = \n";
	cout << "\t\t\t" << vk::to_string(surfaceCapabilities.supportedCompositeAlpha) << "\n";

	cout << "\t\tsupportedUsageFlags: count = \n";
	cout << "\t\t\t" << vk::to_string(surfaceCapabilities.supportedUsageFlags) << "\n";

	cout << "\n\n";
}

void printXorgSurface(vk::Instance instance, vk::PhysicalDevice physicalDevice) {
	// Use XCB to query surface
	int screenNum;
	xcb_connection_t* connection = xcb_connect(NULL, &screenNum);
	if (xcb_connection_has_error(connection) != 0) {
		return;
	}

	const xcb_setup_t* setup = xcb_get_setup(connection);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
	for (int i = 0; i < screenNum; i++) {
		xcb_screen_next(&iter);
	}
	xcb_screen_t* screen = iter.data;
	xcb_window_t window = screen->root;

	vk::XcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.connection = connection;
	surfaceCreateInfo.window = window;

	auto xcbSurface = instance.createXcbSurfaceKHR(surfaceCreateInfo);

	cout << "\tSurface types: count = 2\n";
	cout << "\t\tVK_KHR_xcb_surface\n";
	cout << "\t\tVK_KHR_xlib_surface\n";

	xcb_visualid_t visual_id = screen->root_visual;
	auto presentationSupport = physicalDevice.getXcbPresentationSupportKHR(0, connection, visual_id);
	printSurfaceCapabilities(physicalDevice, xcbSurface);

	instance.destroySurfaceKHR(xcbSurface);

	xcb_disconnect(connection);
}

void printWaylandSurface(vk::Instance instance, vk::PhysicalDevice physicalDevice) {
	// Connect to wayland Display
	wl_display* display = wl_display_connect(NULL);

	if (display == NULL) {
		return;
	}

	// Get registry
	wl_registry* registry = wl_display_get_registry(display);

	wl_registry_listener registry_listener = {
		[](void* data, wl_registry* registry, uint32_t id, const char* interface, uint32_t version) {
			// std::cout << "Got a registry event for " << interface << " id " << id << "\n";
			if (strcmp(interface, "wl_compositor") == 0) {
				*(static_cast<wl_compositor**>(data)) = static_cast<wl_compositor*>(wl_registry_bind(registry, id, &wl_compositor_interface, 1));
			}
		},
		[](void* data, wl_registry* registry, uint32_t id) {}
	};
	wl_compositor* compositor = NULL;
	wl_registry_add_listener(registry, &registry_listener, &compositor);

	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	if (compositor == NULL) {
		return;
	}

	wl_surface* surface = wl_compositor_create_surface(compositor);

	if (surface == NULL) {
		return;
	}

	cout << "\tSurface types: count = 1\n";
	cout << "\t\tVK_KHR_wayland_surface\n";

	auto presentationSupport = physicalDevice.getWaylandPresentationSupportKHR(0, display);

	vk::WaylandSurfaceCreateInfoKHR waylandSurfaceCreateInfo;
	waylandSurfaceCreateInfo.display = display;
	waylandSurfaceCreateInfo.surface = surface;
	auto waylandSurface = instance.createWaylandSurfaceKHR(waylandSurfaceCreateInfo);

	printSurfaceCapabilities(physicalDevice, waylandSurface);

	// Destroy surface
	instance.destroySurfaceKHR(waylandSurface);

	// Disconnect wayland display
	wl_display_disconnect(display);
}

void printPhysicalDevicePresentableSurface(vk::Instance instance, vk::PhysicalDevice physicalDevice) {
	// Linux surfaces
#ifdef USE_LINUX_OPERATING_SYSTEM
	// Xorg
	printXorgSurface(instance, physicalDevice);
	// Wayland
	printWaylandSurface(instance, physicalDevice);
#endif
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

	// Surfaces
	cout << "Presentable Surfaces:\n";
	cout << "=====================\n";
	int deviceId = 0;
	for (auto physicalDevice : physicalDevices) {
		cout << "GPU id : " << deviceId++ << "(" << physicalDevice.getProperties().deviceName << "):\n";
		printPhysicalDevicePresentableSurface(instance, physicalDevice);
	}

	cout << "\nVulkan enabled physical devices :\n";
	for (auto physicalDevice : physicalDevices) {
		auto properties = physicalDevice.getProperties();

		auto features = physicalDevice.getFeatures();

		auto memoryProperties = physicalDevice.getMemoryProperties();

		printQueueFamilies(physicalDevice);
	}

	// Take first physical device for device creation


	// End of the program, destroy Vulkan objects
	instance.destroy();

	return 0;

}
