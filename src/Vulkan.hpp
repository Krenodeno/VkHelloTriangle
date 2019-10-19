#ifndef VULKAN_LOADER_HPP
#define VULKAN_LOADER_HPP

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

#include "OperatingSystem.hpp"

/**
 * Load the vulkan's shared library at runtime.
 * Load core functions symbols only
 */
class VulkanLoader {
public:

	VulkanLoader();
	~VulkanLoader();

	OS::LibraryHandle getLibraryHandle();

	void loadVulkanLibrary();
	void loadExportedEntryPoints();
	void loadGlobalLevelEntryPoints();
	void loadInstanceLevelEntryPoints(vk::Instance);
	void loadDeviceLevelEntryPoints(vk::Device);

	PFN_vkVoidFunction loadInstanceEntryPoint(vk::Instance instance, std::string& funName);
	PFN_vkVoidFunction loadDeviceEntryPoint(vk::Device device, std::string& funName);

	/**
	 * Core function symbols
	 */
#define VK_EXPORTED_FUNCTION( fun ) PFN_##fun fun;
#include "ExportedFunctionList.inl"

#define VK_GLOBAL_LEVEL_FUNCTION( fun ) PFN_##fun fun;
#include "GlobalLevelFunctionList.inl"

#define VK_INSTANCE_LEVEL_FUNCTION( fun ) PFN_##fun fun;
#include "InstanceLevelFunctionList.inl"

#define VK_DEVICE_LEVEL_FUNCTION( fun ) PFN_##fun fun;
#include "DeviceLevelFunctionList.inl"

private:
	OS::LibraryHandle VulkanLibrary;
};

#endif