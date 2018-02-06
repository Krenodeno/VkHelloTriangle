#ifndef VULKAN_LOADER_H
#define VULKAN_LOADER_H

#include <Windows.h>
#include <iostream>

#include <vulkan/vulkan.h>

#include "VulkanFunctions.hpp"

class VulkanLoader {
public:
	VulkanLoader();
	~VulkanLoader();

	bool load();
	bool unload();

	bool isLoaded() const;

	bool LoadExportedEntryPoints();
	bool LoadGlobalEntryPoints();

private:
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	HMODULE vkLibHandle;
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
	void* vkLibHandle;
#endif

};

#endif // !VULKAN_LOADER_H

