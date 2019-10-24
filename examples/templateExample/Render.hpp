#ifndef RENDER_HPP
#define RENDER_HPP

#include "RenderUtils.hpp"
#include "RenderTarget.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"

/* used as callback function to delegate surface creation */
using createSurfaceFoncter = std::function<vk::SurfaceKHR(vk::Instance, vk::DispatchLoaderDynamic)>;

class Render {
public:
	Render(const std::string& appName = "", uint32_t appVersion = 0u, RenderType type = RenderTypeBits::eGraphics);
	~Render();

	void setAppName(const std::string&);
	void setAppVersion(uint32_t);
	void setExtent(const vk::Extent2D&);
	void setRenderType(RenderType);
	void setSurfaceCreationFunction(createSurfaceFoncter);
	void enableValidationLayer();

	void addInstanceExtension(const char*);
	void addDeviceExtension(const char*);
	void addLayer(const char*);

	void waitDeviceIdle();
	void waitForFences();

	void init();
	void drawFrame();

private:

	std::string appName;
	uint32_t appVersion;
	RenderType renderType;
	bool validationLayerEnabled;

	void cleanup();

	VulkanLoader loader;

	std::vector<const char*> layers;
	std::vector<const char*> instanceExtensions;

	vk::UniqueHandle<vk::Instance, VulkanLoader> instance;
	vk::DispatchLoaderDynamic instanceLoader;

	vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic> messenger;

	vk::PhysicalDevice physicalDevice;

	std::vector<const char*> deviceExtensions;
	std::vector<vk::PhysicalDeviceFeatures> deviceFeatures;

	vk::UniqueHandle<vk::Device , vk::DispatchLoaderDynamic> device;
	vk::DispatchLoaderDynamic deviceLoader;

	vk::Queue graphicQueue;
	vk::Queue computeQueue;

	vk::Queue presentQueue;

	createSurfaceFoncter surfaceCreation;
	vk::Extent2D windowExtent;

	vk::UniqueHandle<vk::SurfaceKHR , vk::DispatchLoaderDynamic> surface;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	RenderTarget<vk::DispatchLoaderDynamic> swapchain;

	// Add your objects here

	void createInstance();
	void setupDebugCallback();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSurface();

	void createSwapchain(vk::SwapchainKHR = nullptr);

	// Add your functions here
};

#endif