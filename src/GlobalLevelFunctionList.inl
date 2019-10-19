#ifndef VK_GLOBAL_LEVEL_FUNCTION
#define VK_GLOBAL_LEVEL_FUNCTION( fun )
#endif

/**
 * VkResult vkCreateInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
 */
VK_GLOBAL_LEVEL_FUNCTION(vkCreateInstance)

/**
 * VkResult vkEnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties)
 */
VK_GLOBAL_LEVEL_FUNCTION(vkEnumerateInstanceExtensionProperties)

/**
 * VkResult vkEnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties)
 */
VK_GLOBAL_LEVEL_FUNCTION(vkEnumerateInstanceLayerProperties)

#ifdef VK_VERSION_1_1
/**
 * VkResult vkEnumerateInstanceVersion(uint32_t* pApiVersion)
 */
VK_GLOBAL_LEVEL_FUNCTION(vkEnumerateInstanceVersion)
#endif

#undef VK_GLOBAL_LEVEL_FUNCTION