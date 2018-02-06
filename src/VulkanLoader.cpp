#include "VulkanLoader.hpp"

VulkanLoader::VulkanLoader() : vkLibHandle(nullptr)
{
}

VulkanLoader::~VulkanLoader()
{
	unload();
}

bool VulkanLoader::load()
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	vkLibHandle = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
	vkLibHandle = dlopen("libvulkan.so.1", RTLD_NOW);
#endif

	if (vkLibHandle == nullptr)
		return false;

	return true;
}

bool VulkanLoader::unload()
{
	bool freeResult;
	if (vkLibHandle != nullptr)
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		freeResult = FreeLibrary(vkLibHandle);
#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		freeResult = dlclose(vkLibHandle);
#endif
	vkLibHandle = nullptr;
	return freeResult;
}

bool VulkanLoader::isLoaded() const
{
	return vkLibHandle != nullptr;
}

bool VulkanLoader::LoadExportedEntryPoints()
{
	#if defined(VK_USE_PLATFORM_WIN32_KHR)
		#define LoadProcAddr GetProcAddress
	#elif defined(VK_USE_PLATFORM_XCB_KHR) || defined(VK_USE_PLATFORM_XLIB_KHR)
		#define LoadProcAddr dlsym
	#endif

	#define VK_EXPORTED_FUNCTION( fun ) \
	if ( !(fun = (PFN_##fun)LoadProcAddr(vkLibHandle, #fun)) ) {\
		std::cout << "Could not load exported function: " << #fun << "!" << std::endl;\
		return false;\
	}

	#include "ExportedFunctionList.inl"

	return true;
}

bool VulkanLoader::LoadGlobalEntryPoints()
{
	#define VK_GLOBAL_LEVEL_FUNCTION( fun ) \
	if ( !(fun = (PFN_##fun)vkGetInstanceProcAddr(nullptr, #fun)) ) {\
		std::cout << "Could not load global function: " << #fun << "!" << std::endl;\
		return false;\
	}

	#include "GlobalLevelFunctionList.inl"

	return true;
}
