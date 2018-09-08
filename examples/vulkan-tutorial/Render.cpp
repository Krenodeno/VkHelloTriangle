#include "Render.hpp"

#include <ostream>

Render::Render() {

}

Render::~Render() {
	cleanup();
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	//if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
	std::ostream& out = [&]() -> std::ostream& {
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			return (std::cout);
		if (messageSeverity == VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			return (std::cerr);
		return (std::clog);
	}();

	if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
		out << "[GENERAL]     ";
	if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
		out << "[PERFORMANCE] ";
	if (messageType & VkDebugUtilsMessageTypeFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		out << "[VALIDATION]  ";

	std::cerr << pCallbackData->pMessage << "\n";

	return VK_FALSE;
}

void Render::init() {
	createInstance();
	setupDebugCallback();
}

void Render::cleanup() {
	// Destroy instance realted objects
	vk::DispatchLoaderDynamic didy(instance);
	instance.destroyDebugUtilsMessengerEXT(callback, nullptr, didy);
	// Then destroy the instance
	instance.destroy();
}

void Render::addLayer(const char* layerName) {
	layers.push_back(layerName);
}

void Render::addExtension(const char* extensionName) {
	extensions.push_back(extensionName);
}

void Render::enableValidationLayer() {
	validationLayerEnabled = true;
	layers.push_back("VK_LAYER_LUNARG_standard_validation");
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}

void Render::createInstance() {

	std::cout << "Instance creation\n";
	std::cout << "Required layers: \n";
	for (auto layer : layers) {
		std::cout << "\t" << layer << "\t";
		if (!checkLayerSupport(layer)) {
			std::string error_msg1(layer);
			std::string error_msg2(" not supported !");
			throw std::runtime_error(error_msg1 + error_msg2);
		}
		std::cout << "\n";
	}

	std::cout << "Required extensions: \n";
	for (auto extension: extensions) {
		std::cout << "\t" << extension << (checkExtensionSupport(extension)?"\tsupported":"\tnon supported") << "\n";
	}

	vk::ApplicationInfo appInfo;
	appInfo.pApplicationName = "vulkan-tutorial";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "HelloTriangle";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo instanceInfo;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = layers.size();
	instanceInfo.ppEnabledLayerNames = layers.data();
	instanceInfo.enabledExtensionCount = extensions.size();
	instanceInfo.ppEnabledExtensionNames = extensions.data();

	instance = vk::createInstance(instanceInfo, nullptr, dispatchLoader);
}

void Render::setupDebugCallback() {
	if (!validationLayerEnabled) return;

	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	createInfo.messageSeverity =
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
	createInfo.messageType =
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = nullptr;

	vk::DispatchLoaderDynamic didy(instance);
	callback = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, didy);
}

bool Render::checkExtensionSupport(const char* extensionName) {
	bool result = false;
	for (auto extension : vk::enumerateInstanceExtensionProperties(std::string(), dispatchLoader)) {
		if (strcmp(extensionName, extension.extensionName) == 0) {
			result = true;
			break;
		}
	}
	return result;
}

bool Render::checkLayerSupport(const char* layerName) {
	bool result = false;
	for (auto layer : vk::enumerateInstanceLayerProperties(dispatchLoader)) {
		if (strcmp(layerName, layer.layerName)) {
			result = true;
			break;
		}
	}
	return result;
}
