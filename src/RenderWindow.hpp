#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_NONE
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(USE_LINUX_OPERATING_SYSTEM)
#define GLFW_EXPOSE_NATIVE_WAYLAND
#define GLFW_EXPOSE_NATIVE_X11
#endif // USE_WINDOWS_OPERATING_SYSTEM
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <functional>
#include <iostream>
#include <vector>
#include <string>

void error_callback(int error, const char* description);

class RenderWindow
{
public:
	RenderWindow(int width, int height, std::string name);
	~RenderWindow();

	template<typename Dispatch = vk::DispatchLoaderStatic>
	vk::UniqueSurfaceKHR createSurfaceUnique(vk::Instance instance, Dispatch dispatch = Dispatch()) {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
		vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
		surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
		return instance.createWin32SurfaceKHRUnique(surfaceCreateInfo, nullptr, dispatch);
#elif defined(USE_LINUX_OPERATING_SYSTEM)
		vk::UniqueSurfaceKHR surface;
		vk::WaylandSurfaceCreateInfoKHR WaylandSurfaceCreateInfo;
		WaylandSurfaceCreateInfo.display = glfwGetWaylandDisplay();
		WaylandSurfaceCreateInfo.surface = glfwGetWaylandWindow(window);
		try {
			surface = instance.createWaylandSurfaceKHRUnique(WaylandSurfaceCreateInfo, nullptr, dispatch);
		} catch (vk::SystemError& e) {
			std::cout << "Wayland not supported\n";
		}
		// No error, go for Wayland
		if (surface)
			return surface;

		// Else try X11
		//vk::XlibSurfaceCreateInfoKHR X11surfaceCreateInfo;
		//X11surfaceCreateInfo.dpy = glfwGetX11Display();
		//X11surfaceCreateInfo.window = glfwGetX11Window(window);
		//return instance.createXlibSurfaceKHRUnique(X11surfaceCreateInfo, nullptr, dispatch);
#endif // USE_WINDOWS_OPERATING_SYSTEM
	}

	template<typename Dispatch = vk::DispatchLoaderStatic>
	vk::SurfaceKHR createSurface(vk::Instance instance, Dispatch dispatch = Dispatch()) {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
		vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
		surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
		return instance.createWin32SurfaceKHR(surfaceCreateInfo, nullptr, dispatch);
#elif defined(USE_LINUX_OPERATING_SYSTEM)
		vk::SurfaceKHR surface;
		vk::WaylandSurfaceCreateInfoKHR WaylandSurfaceCreateInfo;
		WaylandSurfaceCreateInfo.display = glfwGetWaylandDisplay();
		WaylandSurfaceCreateInfo.surface = glfwGetWaylandWindow(window);
		try {
			surface = instance.createWaylandSurfaceKHR(WaylandSurfaceCreateInfo, nullptr, dispatch);
		} catch (vk::SystemError& e) {
			std::cout << "Wayland not supported\n";
		}
		// No error, go for Wayland
		if (surface)
			return surface;

		// Else try X11
		//vk::XlibSurfaceCreateInfoKHR surfaceCreateInfo;
		//surfaceCreateInfo.dpy = glfwGetX11Display();
		//surfaceCreateInfo.window = glfwGetX11Window(window);
		//return instance.createXlibSurfaceKHR(surfaceCreateInfo, nullptr, dispatch);
#endif // USE_WINDOWS_OPERATING_SYSTEM
	}

	int getWidth();
	int getHeight();
	std::vector<const char*> getRequiredExtensions();

	bool isClosed();

	void setUserPointer(void* ptr);
	void setResizeCallback(GLFWwindowsizefun resizeFunction);

	void pollEvents();

private:
	GLFWwindow * window;
	int width;
	int height;
};

#endif // !RENDER_WINDOW_HPP
