#include "VulkanLoader.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>

VulkanLoader::VulkanLoader() : vkLibHandle(nullptr)
{
}

VulkanLoader::~VulkanLoader()
{
	if (!unload())
		std::cout << "Error unloading Vulkan library : ";
		#if defined(VK_USE_PLATFORM_WIN32_KHR)
		std::cout << GetLastError() << "\n";
		#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		std::cout << dlerror() << "\n";
		#endif
}

bool VulkanLoader::load()
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	vkLibHandle = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
	vkLibHandle = dlopen("libvulkan.so.1", RTLD_NOW);
#endif

	if (vkLibHandle == nullptr)
		return false;

	return true;
}

bool VulkanLoader::unload()
{
	bool freeResult = true;
	if (vkLibHandle != nullptr)
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		freeResult = FreeLibrary(reinterpret_cast<HMODULE>(vkLibHandle));
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		freeResult = dlclose(vkLibHandle);
#endif
	vkLibHandle = nullptr;
	return freeResult;
}

bool VulkanLoader::isLoaded() const
{
	return vkLibHandle != nullptr;
}

bool VulkanLoader::LoadExportedEntryPoints()
{
	#if defined(VK_USE_PLATFORM_WIN32_KHR)
		#define LoadProcAddr(handle, fun) GetProcAddress(reinterpret_cast<HMODULE>(handle), fun)
	#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		#define LoadProcAddr dlsym
	#endif

	#define VK_EXPORTED_FUNCTION( fun ) \
	if ( !(fun = (PFN_##fun) LoadProcAddr(vkLibHandle, #fun)) ) {\
		std::cout << "Could not load exported function: " << #fun << "!\n";\
		return false;\
	}

	#include "ExportedFunctionList.inl"

	return true;
}

bool VulkanLoader::LoadGlobalEntryPoints()
{
	#define VK_GLOBAL_LEVEL_FUNCTION( fun ) \
	if ( !(fun = (PFN_##fun) vkGetInstanceProcAddr(nullptr, #fun)) ) {\
		std::cout << "Could not load global level function: " << #fun << "!\n";\
		return false;\
	}

	#include "GlobalLevelFunctionList.inl"

	return true;
}

bool VulkanLoader::loadInstanceEntryPoints(VkInstance instance)
{
	#define VK_INSTANCE_LEVEL_FUNCTION( fun ) \
	if ( !(fun = (PFN_##fun) vkGetInstanceProcAddr(instance, #fun)) ) {\
		std::cout << "Could not load instance level function: " << #fun << "!\n";\
		return false;\
	}

	#include "InstanceLevelFunctionList.inl"

	return true;
}

bool VulkanLoader::loadDeviceEntryPoints(VkDevice device)
{
#define VK_DEVICE_LEVEL_FUNCTION( fun ) \
	if ( !(fun = (PFN_##fun) vkGetDeviceProcAddr(device, #fun))) {\
		std::cout << "Could not load device level function: " << #fun << "!\n";\
		return false;\
	}
	return true;
}
