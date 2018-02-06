#ifndef VULKAN_FUNCTIONS_H
#define VULKAN_FUNCTIONS_H

#include <vulkan/vulkan.h>

#define VK_EXPORTED_FUNCTION( fun )       extern PFN_##fun fun;
#define VK_GLOBAL_LEVEL_FUNCTION( fun )   extern PFN_##fun fun;
#define VK_INSTANCE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;
#define VK_DEVICE_LEVEL_FUNCTION( fun )   extern PFN_##fun fun;

// Exported function
#include "ExportedFunctionList.inl"
// Global level functions
#include "GlobalLevelFunctionList.inl"
// Instance level functions
#include "InstanceLevelFunctionList.inl"
// Device level functions
#include "DeviceLevelFunctionList.inl"

#endif // !VULKAN_FUNCTIONS_H
