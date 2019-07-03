#ifndef RENDER_UTILS_HPP
#define RENDER_UTILS_HPP

#include "Vulkan.hpp"

#include <optional>
#include <set>
#include <vector>


enum class RenderTypeBits : unsigned int {
	eGraphics = 0x00000001,
	eCompute  = 0x00000002
};

class RenderType {
public:
	RenderType(RenderTypeBits);
	~RenderType() = default;
	RenderType(const RenderType&);

	RenderType& operator=(const RenderType&);
	RenderType& operator|=(const RenderType&);
	RenderType& operator&=(const RenderType&);
	RenderType& operator^=(const RenderType&);
	RenderType operator|(const RenderType&) const;
	RenderType operator&(const RenderType&) const;
	RenderType operator^(const RenderType&) const;
	bool operator!() const;
	bool operator==(const RenderType&) const;
	bool operator!=(const RenderType&) const;
	explicit operator bool() const;
private:
	unsigned int flags;
};

RenderType operator|(RenderTypeBits, const RenderType&);
RenderType operator&(RenderTypeBits, const RenderType&);
RenderType operator^(RenderTypeBits, const RenderType&);

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> computeFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && computeFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

template<typename Dispatch = vk::DispatchLoaderDefault>
inline bool checkExtensionSupport(const char* extension, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline bool checkLayerSupport(const char* layer, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline bool checkDeviceExtensionSupport(vk::PhysicalDevice, std::vector<const char*>, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice, vk::SurfaceKHR = nullptr, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline bool isDeviceSuitable(vk::PhysicalDevice, std::vector<const char*> extensions, vk::QueueFlags, vk::SurfaceKHR = nullptr, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice, vk::SurfaceKHR, const Dispatch& d = Dispatch());
inline vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>&);
inline vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>&);
inline vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR&, vk::Extent2D);
template<typename Dispatch = vk::DispatchLoaderDefault>
inline uint32_t findMemoryType(vk::PhysicalDevice, uint32_t typeFilter, vk::MemoryPropertyFlags, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline vk::Format findSupportedFormat(vk::PhysicalDevice, const std::vector<vk::Format>&, vk::ImageTiling, vk::FormatFeatureFlags, const Dispatch& d = Dispatch());
template<typename Dispatch = vk::DispatchLoaderDefault>
inline vk::Format findDepthFormat(vk::PhysicalDevice, const Dispatch& d = Dispatch());
inline bool hasStencilComponent(vk::Format);
template<typename Dispatch = vk::DispatchLoaderDefault>
inline void fillBuffer(vk::Device, vk::DeviceMemory&, const void*, vk::DeviceSize, const Dispatch& d = Dispatch());
template<typename T, typename Dispatch = vk::DispatchLoaderDefault>
inline void fillBuffer(vk::Device, vk::DeviceMemory&, std::vector<T>, const Dispatch& d = Dispatch());

#include "RenderUtils.inl"

#endif
