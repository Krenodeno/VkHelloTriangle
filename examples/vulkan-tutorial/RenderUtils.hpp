#ifndef RENDER_UTILS_HPP
#define RENDER_UTILS_HPP

#include <vulkan/vulkan.hpp>
#include <optional>
#include <set>
#include <vector>

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

inline bool checkExtensionSupport(const char*);
inline bool checkLayerSupport(const char*);
inline bool checkDeviceExtensionSupport(vk::PhysicalDevice, std::vector<const char*>);
inline QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice, vk::SurfaceKHR);
inline bool isDeviceSuitable(vk::PhysicalDevice, vk::SurfaceKHR, std::vector<const char*>);
inline SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice, vk::SurfaceKHR);
inline vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>&);
inline vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>&);
inline vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR&, vk::Extent2D);


#include "RenderUtils.inl"

#endif
