#ifndef VULKAN_LOADER_HPP
#define VULKAN_LOADER_HPP

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

#include "Instance.hpp"
#include "Device.hpp"

#include "OperatingSystem.hpp"

class VulkanLoader {
public:

	VulkanLoader();
	~VulkanLoader();

	OS::LibraryHandle getLibraryHandle();

	void loadVulkanLibrary();
	void loadExportedEntryPoints();
	void loadGlobalLevelEntryPoints();
	void loadInstanceLevelEntryPoints(Instance);
	void loadDeviceLevelEntryPoints(Device);

	void loadInstanceEntryPoint(std::string& funName);
	void loadDeviceEntryPoint(std::string& funName);

private:
	OS::LibraryHandle VulkanLibrary;
};

#endif