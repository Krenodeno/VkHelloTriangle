#ifndef VK_INSTANCE_HPP
#define VK_INSTANCE_HPP

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

class Instance {
public:
	Instance() : handle(nullptr) {}
	~Instance() {}

	Instance(const Instance&) = delete;
	Instance(Instance&&) = default;

	Instance& operator=(const Instance&) = delete;
	Instance& operator=(Instance&&) = default;

	Instance(vk::Instance instance) : handle(instance) {
	}

	Instance& operator=(vk::Instance) {

	}

	operator vk::Instance() const { return handle; }

#ifdef VK_USE_PLATFORM_ANDROID_KHR
	vk::Result createAndroidSurfaceKHR( const AndroidSurfaceCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface) const {
		return handle.createAndroidSurfaceKHR(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	vk::ResultValueType< vk::SurfaceKHR>::type createAndroidSurfaceKHR( const AndroidSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr) const {
		return handle.createAndroidSurfaceKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createAndroidSurfaceKHRUnique( const AndroidSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr) const {
		return handle.createAndroidSurfaceKHR(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

	vk::Result createDebugReportCallbackEXT( const vk::DebugReportCallbackCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugReportCallbackEXT* pCallback ) const {
		return handle.createDebugReportCallbackEXT(pCreateInfo, pAllocator, pCallback, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType<vk::DebugReportCallbackEXT>::type createDebugReportCallbackEXT( const vk::DebugReportCallbackCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createDebugReportCallbackEXT(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle<vk::DebugReportCallbackEXT, vk::DispatchLoaderDynamic >>::type createDebugReportCallbackEXTUnique( const vk::DebugReportCallbackCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createDebugReportCallbackEXTUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result createDebugUtilsMessengerEXT( const vk::DebugUtilsMessengerCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DebugUtilsMessengerEXT* pMessenger ) const {
		return handle.createDebugUtilsMessengerEXT(pCreateInfo, pAllocator, pMessenger, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::DebugUtilsMessengerEXT>::type createDebugUtilsMessengerEXT( const vk::DebugUtilsMessengerCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createDebugUtilsMessengerEXT(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic >>::type createDebugUtilsMessengerEXTUnique( const vk::DebugUtilsMessengerCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createDebugUtilsMessengerEXTUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result createDisplayPlaneSurfaceKHR( const vk::DisplaySurfaceCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createDisplayPlaneSurfaceKHR(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createDisplayPlaneSurfaceKHR( const vk::DisplaySurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createDisplayPlaneSurfaceKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createDisplayPlaneSurfaceKHRUnique( const vk::DisplaySurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createDisplayPlaneSurfaceKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result createHeadlessSurfaceEXT( const vk::HeadlessSurfaceCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createHeadlessSurfaceEXT(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createHeadlessSurfaceEXT( const vk::HeadlessSurfaceCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createHeadlessSurfaceEXT(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createHeadlessSurfaceEXTUnique( const vk::HeadlessSurfaceCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createHeadlessSurfaceEXTUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_IOS_MVK

	vk::Result createIOSSurfaceMVK( const vk::IOSSurfaceCreateInfoMVK* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createIOSSurfaceMVK(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createIOSSurfaceMVK( const vk::IOSSurfaceCreateInfoMVK & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createIOSSurfaceMVK(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createIOSSurfaceMVKUnique( const vk::IOSSurfaceCreateInfoMVK & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createIOSSurfaceMVKUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_IOS_MVK*/

#ifdef VK_USE_PLATFORM_FUCHSIA

	vk::Result createImagePipeSurfaceFUCHSIA( const vk::ImagePipeSurfaceCreateInfoFUCHSIA* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createImagePipeSurfaceFUCHSIA(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createImagePipeSurfaceFUCHSIA( const vk::ImagePipeSurfaceCreateInfoFUCHSIA & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createImagePipeSurfaceFUCHSIA(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createImagePipeSurfaceFUCHSIAUnique( const vk::ImagePipeSurfaceCreateInfoFUCHSIA & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createImagePipeSurfaceFUCHSIAUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_FUCHSIA*/

#ifdef VK_USE_PLATFORM_MACOS_MVK

	vk::Result createMacOSSurfaceMVK( const vk::MacOSSurfaceCreateInfoMVK* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createMacOSSurfaceMVK(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createMacOSSurfaceMVK( const vk::MacOSSurfaceCreateInfoMVK & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createMacOSSurfaceMVK(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createMacOSSurfaceMVKUnique( const vk::MacOSSurfaceCreateInfoMVK & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createMacOSSurfaceMVKUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_MACOS_MVK*/

#ifdef VK_USE_PLATFORM_METAL_EXT

	vk::Result createMetalSurfaceEXT( const vk::MetalSurfaceCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createMetalSurfaceEXT(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createMetalSurfaceEXT( const vk::MetalSurfaceCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createMetalSurfaceEXT(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createMetalSurfaceEXTUnique( const vk::MetalSurfaceCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createMetalSurfaceEXTUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_METAL_EXT*/

#ifdef VK_USE_PLATFORM_GGP

	vk::Result createStreamDescriptorSurfaceGGP( const vk::StreamDescriptorSurfaceCreateInfoGGP* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createStreamDescriptorSurfaceGGP(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createStreamDescriptorSurfaceGGP( const vk::StreamDescriptorSurfaceCreateInfoGGP & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createStreamDescriptorSurfaceGGP(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createStreamDescriptorSurfaceGGPUnique( const vk::StreamDescriptorSurfaceCreateInfoGGP & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createStreamDescriptorSurfaceGGPUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_GGP*/

#ifdef VK_USE_PLATFORM_VI_NN

	vk::Result createViSurfaceNN( const vk::ViSurfaceCreateInfoNN* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createViSurfaceNN(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createViSurfaceNN( const vk::ViSurfaceCreateInfoNN & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createViSurfaceNN(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createViSurfaceNNUnique( const vk::ViSurfaceCreateInfoNN & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createViSurfaceNNUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_VI_NN*/

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

	vk::Result createWaylandSurfaceKHR( const vk::WaylandSurfaceCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createWaylandSurfaceKHR(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createWaylandSurfaceKHR( const vk::WaylandSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createWaylandSurfaceKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createWaylandSurfaceKHRUnique( const vk::WaylandSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createWaylandSurfaceKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WAYLAND_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR

	vk::Result createWin32SurfaceKHR( const vk::Win32SurfaceCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createWin32SurfaceKHR(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createWin32SurfaceKHR( const vk::Win32SurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createWin32SurfaceKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createWin32SurfaceKHRUnique( const vk::Win32SurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createWin32SurfaceKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_XCB_KHR

	vk::Result createXcbSurfaceKHR( const vk::XcbSurfaceCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createXcbSurfaceKHR(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createXcbSurfaceKHR( const vk::XcbSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createXcbSurfaceKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createXcbSurfaceKHRUnique( const vk::XcbSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createXcbSurfaceKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_XCB_KHR*/

#ifdef VK_USE_PLATFORM_XLIB_KHR

	vk::Result createXlibSurfaceKHR( const vk::XlibSurfaceCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SurfaceKHR* pSurface ) const {
		return handle.createXlibSurfaceKHR(pCreateInfo, pAllocator, pSurface, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType< vk::SurfaceKHR>::type createXlibSurfaceKHR( const vk::XlibSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createXlibSurfaceKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle< vk::SurfaceKHR, vk::DispatchLoaderDynamic >>::type createXlibSurfaceKHRUnique( const vk::XlibSurfaceCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		return handle.createXlibSurfaceKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_XLIB_KHR*/


	void debugReportMessageEXT( vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage ) const {
		handle.debugReportMessageEXT(flags, objectType, object, location, messageCode, pLayerPrefix, pMessage, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void debugReportMessageEXT( vk::DebugReportFlagsEXT flags, vk::DebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const std::string & layerPrefix, const std::string & message ) const {
		handle.debugReportMessageEXT(flags, objectType, object, location, messageCode, layerPrefix, message, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroyDebugReportCallbackEXT( vk::DebugReportCallbackEXT callback, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyDebugReportCallbackEXT(callback, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroyDebugReportCallbackEXT( vk::DebugReportCallbackEXT callback, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyDebugReportCallbackEXT(callback, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroy( vk::DebugReportCallbackEXT callback, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(callback, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroy( vk::DebugReportCallbackEXT callback, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(callback, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroyDebugUtilsMessengerEXT( vk::DebugUtilsMessengerEXT messenger, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyDebugUtilsMessengerEXT(messenger, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroyDebugUtilsMessengerEXT( vk::DebugUtilsMessengerEXT messenger, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyDebugUtilsMessengerEXT(messenger, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroy( vk::DebugUtilsMessengerEXT messenger, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(messenger, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroy( vk::DebugUtilsMessengerEXT messenger, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(messenger, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroy( const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroy( vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroySurfaceKHR( vk::SurfaceKHR surface, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroySurfaceKHR(surface, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroySurfaceKHR( vk::SurfaceKHR surface, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroySurfaceKHR(surface, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void destroy( vk::SurfaceKHR surface, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(surface, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void destroy( vk::SurfaceKHR surface, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(surface, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result enumeratePhysicalDeviceGroups( uint32_t* pPhysicalDeviceGroupCount,  vk::PhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties ) const {
		handle.enumeratePhysicalDeviceGroups(pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator< vk::PhysicalDeviceGroupProperties>>
	typename vk::ResultValueType<std::vector< vk::PhysicalDeviceGroupProperties,Allocator>>::type enumeratePhysicalDeviceGroups() const {
		handle.enumeratePhysicalDeviceGroups(dispatch);
	}
	template<typename Allocator = std::allocator< vk::PhysicalDeviceGroupProperties>>
	typename vk::ResultValueType<std::vector< vk::PhysicalDeviceGroupProperties,Allocator>>::type enumeratePhysicalDeviceGroups(Allocator const& vectorAllocator) const {
		handle.enumeratePhysicalDeviceGroups(vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result enumeratePhysicalDeviceGroupsKHR( uint32_t* pPhysicalDeviceGroupCount,  vk::PhysicalDeviceGroupProperties* pPhysicalDeviceGroupProperties ) const {
		handle.enumeratePhysicalDeviceGroupsKHR(pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator< vk::PhysicalDeviceGroupProperties>>
	typename vk::ResultValueType<std::vector< vk::PhysicalDeviceGroupProperties,Allocator>>::type enumeratePhysicalDeviceGroupsKHR() const {
		handle.enumeratePhysicalDeviceGroupsKHR(dispatch);
	}
	template<typename Allocator = std::allocator< vk::PhysicalDeviceGroupProperties>>
	typename vk::ResultValueType<std::vector< vk::PhysicalDeviceGroupProperties,Allocator>>::type enumeratePhysicalDeviceGroupsKHR(Allocator const& vectorAllocator) const {
		handle.enumeratePhysicalDeviceGroupsKHR(vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result enumeratePhysicalDevices( uint32_t* pPhysicalDeviceCount, vk::PhysicalDevice* pPhysicalDevices ) const {
		handle.enumeratePhysicalDevices(pPhysicalDeviceCount, pPhysicalDevices, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::PhysicalDevice>>
	typename vk::ResultValueType<std::vector<vk::PhysicalDevice,Allocator>>::type enumeratePhysicalDevices() const {
		handle.enumeratePhysicalDevices(dispatch);
	}
	template<typename Allocator = std::allocator<vk::PhysicalDevice>>
	typename vk::ResultValueType<std::vector<vk::PhysicalDevice,Allocator>>::type enumeratePhysicalDevices(Allocator const& vectorAllocator) const {
		handle.enumeratePhysicalDevices(vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	PFN_vkVoidFunction getProcAddr( const char* pName ) const {
		handle.getProcAddr(pName, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	PFN_vkVoidFunction getProcAddr( const std::string & name ) const {
		handle.getProcAddr(name, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	void submitDebugUtilsMessageEXT( vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageTypes, const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData ) const {
		handle.submitDebugUtilsMessageEXT(messageSeverity, messageTypes, pCallbackData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	void submitDebugUtilsMessageEXT( vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity, vk::DebugUtilsMessageTypeFlagsEXT messageTypes, const vk::DebugUtilsMessengerCallbackDataEXT & callbackData ) const {
		handle.submitDebugUtilsMessageEXT(messageSeverity, messageTypes, callbackData, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

private:
	vk::Instance handle;
	vk::DispatchLoaderDynamic dispatch;
};

#endif