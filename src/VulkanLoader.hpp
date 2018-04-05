#ifndef VULKAN_LOADER_H
#define VULKAN_LOADER_H

#if defined(USE_WINDOWS_OPERATING_SYSTEM)
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(USE_LINUX_OPERATING_SYSTEM)
#define VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_WAYLAND_KHR
#endif

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
	bool loadInstanceEntryPoints(VkInstance instance);
	bool loadDeviceEntryPoints(VkDevice device);

private:
	void* vkLibHandle;

};

#endif // !VULKAN_LOADER_H

