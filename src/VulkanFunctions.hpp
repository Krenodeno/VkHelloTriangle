#ifndef VULKAN_FUNCTIONS_HPP
#define VULKAN_FUNCTIONS_HPP

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#define VK_EXPORTED_FUNCTION( fun ) extern PFN_##fun fun;
#include "ExportedFunctionList.inl"

#define VK_GLOBAL_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;
#include "GlobalLevelFunctionList.inl"

#define VK_INSTANCE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;
#include "InstanceLevelFunctionList.inl"

#define VK_DEVICE_LEVEL_FUNCTION( fun ) extern PFN_##fun fun;
#include "DeviceLevelFunctionList.inl"

#endif