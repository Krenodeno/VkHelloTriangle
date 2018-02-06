#include <vulkan/vulkan.h>

#define VK_EXPORTED_FUNCTION( fun )       PFN_##fun fun;
#define VK_GLOBAL_LEVEL_FUNCTION( fun )   PFN_##fun fun;
#define VK_INSTANCE_LEVEL_FUNCTION( fun ) PFN_##fun fun;
#define VK_DEVICE_LEVEL_FUNCTION( fun )   PFN_##fun fun;

// Exported function
#include "ExportedFunctionList.inl"
// Global level functions
#include "GlobalLevelFunctionList.inl"
// Instance level functions
#include "InstanceLevelFunctionList.inl"
// Device level functions
#include "DeviceLevelFunctionList.inl"