#ifndef RENDER_UTILS_HPP
#define RENDER_UTILS_HPP

#include <vulkan/vulkan.hpp>
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

inline bool checkExtensionSupport(const char*);
inline bool checkLayerSupport(const char*);
inline bool checkDeviceExtensionSupport(vk::PhysicalDevice, std::vector<const char*>);
inline QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice, vk::SurfaceKHR);
inline bool isDeviceSuitable(vk::PhysicalDevice, vk::SurfaceKHR, std::vector<const char*>);
inline SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice, vk::SurfaceKHR);
inline vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>&);
inline vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>&);
inline vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR&, vk::Extent2D);
inline uint32_t findMemoryType(vk::PhysicalDevice device, uint32_t typeFilter, vk::MemoryPropertyFlags properties);


#include "RenderUtils.inl"

#endif
