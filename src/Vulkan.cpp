#include "Vulkan.hpp"

VulkanLoader::VulkanLoader() {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
	VulkanLibrary = LoadLibrary("vulkan-1.dll");
#elif defined(USE_LINUX_OPERATING_SYSTEM)
	VulkanLibrary = dlopen("libvulkan.so.1", RTLD_NOW);
#endif

	loadExportedEntryPoints();
	loadGlobalLevelEntryPoints();
}

VulkanLoader::~VulkanLoader() {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
	FreeLibrary(VulkanLibrary);
#elif defined(USE_LINUX_OPERATING_SYSTEM)
	dlclose(VulkanLibrary);
#endif
}

void VulkanLoader::loadExportedEntryPoints() {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
	#define LoadProcAddress GetProcAddress
#elif defined(USE_LINUX_OPERATING_SYSTEM)
	#define LoadProcAddress dlsym
#endif

#define VK_EXPORTED_FUNCTION(fun)\
	if( !(fun = (PFN_##fun)LoadProcAddress(VulkanLibrary, #fun))) {\
		std::string errorMsg ("Could not load exported function: "#fun" !");\
		throw std::runtime_error(errorMsg);\
	}

#include "ExportedFunctionList.inl"

}

void VulkanLoader::loadGlobalLevelEntryPoints() {

#define VK_GLOBAL_LEVEL_FUNCTION(fun)\
	if( !(fun = (PFN_##fun)vkGetInstanceProcAddr(nullptr, #fun))) {\
		std::string errorMsg ("Could not load global level function: "#fun" !");\
		throw std::runtime_error(errorMsg);\
	}

#include "GlobalLevelFunctionList.inl"

}

void VulkanLoader::loadInstanceLevelEntryPoints(vk::Instance instance) {

#define VK_INSTANCE_LEVEL_FUNCTION(fun)\
	if( !(fun = (PFN_##fun)vkGetInstanceProcAddr(VkInstance(instance), #fun))) {\
		std::string errorMsg ("Could not load Instance level function: "#fun" !");\
		throw std::runtime_error(errorMsg);\
	}

#include "InstanceLevelFunctionList.inl"

}

void VulkanLoader::loadDeviceLevelEntryPoints(vk::Device device) {

#define VK_INSTANCE_LEVEL_FUNCTION(fun)\
	if( !(fun = (PFN_##fun)vkGetDeviceProcAddr(VkDevice(device), #fun))) {\
		std::string errorMsg ("Could not load Device level function: "#fun" !");\
		throw std::runtime_error(errorMsg);\
	}

#include "DeviceLevelFunctionList.inl"

}

PFN_vkVoidFunction VulkanLoader::loadInstanceEntryPoint(vk::Instance instance, std::string& funName) {
	PFN_vkVoidFunction fun;
	if (!(fun = vkGetInstanceProcAddr(VkInstance(instance), funName.c_str()))) {
		std::string errorMsg1 ("Could not load Instance level function: ");
		std::string errorMsg2 (" !");
		throw std::runtime_error(errorMsg1 + funName + errorMsg2);
	}
	return fun;
}

PFN_vkVoidFunction VulkanLoader::loadDeviceEntryPoint(vk::Device device, std::string& funName) {
	PFN_vkVoidFunction fun;
	if (!(fun = vkGetDeviceProcAddr(VkDevice(device), funName.c_str()))) {
		std::string errorMsg1 ("Could not load Device level function: ");
		std::string errorMsg2 (" !");
		throw std::runtime_error(errorMsg1 + funName + errorMsg2);
	}
	return fun;
}
