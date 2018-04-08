#ifndef VULKAN_LOADER_H
#define VULKAN_LOADER_H

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

