#include "Vulkan.hpp"
#include "VulkanFunctions.hpp"

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

void VulkanLoader::loadInstanceLevelEntryPoints(Instance instance) {

#define VK_INSTANCE_LEVEL_FUNCTION(fun)\
	if( !(fun = (PFN_##fun)vkGetInstanceProcAddr(instance, #fun))) {\
		std::string errorMsg ("Could not load Instance level function: "#fun" !");\
		throw std::runtime_error(errorMsg);\
	}

#include "InstanceLevelFunctionList.inl"

}

void VulkanLoader::loadDeviceLevelEntryPoints(Device device) {

#define VK_INSTANCE_LEVEL_FUNCTION(fun)\
	if( !(fun = (PFN_##fun)vkGetDeviceProcAddr(device, #fun))) {\
		std::string errorMsg ("Could not load Device level function: "#fun" !");\
		throw std::runtime_error(errorMsg);\
	}

#include "DeviceLevelFunctionList.inl"

}

void VulkanLoader::loadInstanceEntryPoint(std::string& funName) {

}

void VulkanLoader::loadDeviceEntryPoint(std::string& funName) {

}
