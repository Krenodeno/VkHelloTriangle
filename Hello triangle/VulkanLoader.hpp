#ifndef VULKAN_LOADER_H
#define VULKAN_LOADER_H

#include <Windows.h>

#include <vulkan/vulkan.h>

class VulkanLoader {
public:
	VulkanLoader() : vkLibHandle(nullptr) {}
	~VulkanLoader()
	{
		unload();
	}

	bool load() {
		// First load shared lib
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		vkLibHandle = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		vkLibHandle = dlopen("libvulkan.so.1", RTLD_NOW);
#endif // VK_USE_PLATFORM_WIN32_KHR

		if (vkLibHandle == nullptr)
			return false;

		return true;
	}

	bool unload() {
		if (vkLibHandle != nullptr)
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			FreeLibrary(vkLibHandle);
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
			dlclose(vkLibHandle);
#endif // VK_USE_PLATFORM_WIN32_KHR
		vkLibHandle = nullptr;
	}

	bool isLoaded() const {
		return vkLibHandle != nullptr;
	}

	bool LoadExportedEntryPoints() {
		#if defined(VK_USE_PLATFORM_WIN32_KHR)
			#define LoadProcAddr GetProcAddress
		#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
			#define LoadProcAddr dlsym
		#endif

		#define VK_EXPORTED_FUNCTION( fun )\
		if (!(fun = (PFN_##fun)LoadProcAddr(vkLibHandle, ##fun))) {\
			std::cout << "Could not load exported function: " << #fun << "!" << std::endl;\
			return false;\
		}

		#include "ExportedFunctionList.inl"

		return true;
	}

	bool LoadGlobalEntryPoints() {
		#define VK_EXPORTED_FUNCTION( fun )\
		if (!(fun = (PFN_##fun)vkGetInstanceProcAddr(nullptr, ##fun))) {\
			std::cout << "Could not load global function: " << #fun << "!" << std::endl;\
			return false;\
		}

		#include "GlobalLevelFunctionList.inl"

		return true;
	}

private:
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	HMODULE vkLibHandle;
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
	void* vkLibHandle;
#endif

};

#endif // !VULKAN_LOADER_H

