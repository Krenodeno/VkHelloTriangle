#if defined(USE_WINDOWS_OPERATING_SYSTEM)
#	define VK_USE_PLATFORM_WIN32_KHR
#elif defined(USE_LINUX_OPERATING_SYSTEM)
#	define VK_USE_PLATFORM_XLIB_KHR
#	define VK_USE_PLATFORM_WAYLAND_KHR
#endif

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