#include "VulkanLoader.hpp"

#ifdef USE_WINDOWS_OPERATING_SYSTEM
#	include <Windows.h>
#elif defined(USE_LINUX_OPERATING_SYSTEM)
#	include <dlfcn.h>
#endif
#include <iostream>

VulkanLoader::VulkanLoader() : vkLibHandle(nullptr)
{
}

VulkanLoader::~VulkanLoader()
{
	if (!unload())
		std::cout << "Error unloading Vulkan library : ";
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
		std::cout << GetLastError() << "\n";
#elif defined(USE_LINUX_OPERATING_SYSTEM)
		std::cout << dlerror() << "\n";
#endif
}

bool VulkanLoader::load()
{
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
	vkLibHandle = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined(USE_LINUX_OPERATING_SYSTEM)
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
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
		freeResult = FreeLibrary(reinterpret_cast<HMODULE>(vkLibHandle));
#elif defined(USE_LINUX_OPERATING_SYSTEM)
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
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
#	define LoadProcAddr(handle, fun) GetProcAddress(reinterpret_cast<HMODULE>(handle), fun)
#elif defined(USE_LINUX_OPERATING_SYSTEM)
#	define LoadProcAddr dlsym
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

	#include "DeviceLevelFunctionList.inl"

	return true;
}
