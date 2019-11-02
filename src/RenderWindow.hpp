#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include "Vulkan.hpp"

#define GLFW_INCLUDE_NONE
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(USE_LINUX_OPERATING_SYSTEM)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <functional>
#include <iostream>
#include <vector>
#include <string>

/*
regex1 = "#define (GLFW_KEY[_0-9A-Z]+)[ ]+-?[0-9A-Z]*";
regex2 = "_[0-9]*([A-Z])?[A-Z]*";
replace = "	e$2 = $1,"
*/
enum class Key : int {
	eSpace =        GLFW_KEY_SPACE,
	eApostrophe =   GLFW_KEY_APOSTROPHE,
	eComma =        GLFW_KEY_COMMA,
	eMinus =        GLFW_KEY_MINUS,
	ePeriod =       GLFW_KEY_PERIOD,
	eSlash =        GLFW_KEY_SLASH,
	e0 =            GLFW_KEY_0,
	e1 =            GLFW_KEY_1,
	e2 =            GLFW_KEY_2,
	e3 =            GLFW_KEY_3,
	e4 =            GLFW_KEY_4,
	e5 =            GLFW_KEY_5,
	e6 =            GLFW_KEY_6,
	e7 =            GLFW_KEY_7,
	e8 =            GLFW_KEY_8,
	e9 =            GLFW_KEY_9,
	eSemicolon =    GLFW_KEY_SEMICOLON,
	eEqual =        GLFW_KEY_EQUAL,
	eA =            GLFW_KEY_A,
	eB =            GLFW_KEY_B,
	eC =            GLFW_KEY_C,
	eD =            GLFW_KEY_D,
	eE =            GLFW_KEY_E,
	eF =            GLFW_KEY_F,
	eG =            GLFW_KEY_G,
	eH =            GLFW_KEY_H,
	eI =            GLFW_KEY_I,
	eJ =            GLFW_KEY_J,
	eK =            GLFW_KEY_K,
	eL =            GLFW_KEY_L,
	eM =            GLFW_KEY_M,
	eN =            GLFW_KEY_N,
	eO =            GLFW_KEY_O,
	eP =            GLFW_KEY_P,
	eQ =            GLFW_KEY_Q,
	eR =            GLFW_KEY_R,
	eS =            GLFW_KEY_S,
	eT =            GLFW_KEY_T,
	eU =            GLFW_KEY_U,
	eV =            GLFW_KEY_V,
	eW =            GLFW_KEY_W,
	eX =            GLFW_KEY_X,
	eY =            GLFW_KEY_Y,
	eZ =            GLFW_KEY_Z,
	eLeftBracket =  GLFW_KEY_LEFT_BRACKET,
	eBackslash =    GLFW_KEY_BACKSLASH,
	eRightBracket = GLFW_KEY_BACKSLASH,
	eGraveAccent =  GLFW_KEY_BACKSLASH,
	eWorld1 =       GLFW_KEY_WORLD_1,
	eWorld2 =       GLFW_KEY_WORLD_2,

	eEscape =       GLFW_KEY_ESCAPE,
	eEnter =        GLFW_KEY_ENTER,
	eTab =          GLFW_KEY_TAB,
	eBackspace =    GLFW_KEY_BACKSPACE,
	eInsert =       GLFW_KEY_INSERT,
	eDelete =       GLFW_KEY_DELETE,
	eRight =        GLFW_KEY_RIGHT,
	eLeft =         GLFW_KEY_LEFT,
	eDown =         GLFW_KEY_DOWN,
	eUp =           GLFW_KEY_UP,
	ePageUp =       GLFW_KEY_PAGE_UP,
	ePageDown =     GLFW_KEY_PAGE_DOWN,
	eHome =         GLFW_KEY_HOME,
	eEnd =          GLFW_KEY_END,
	eCapsLock =     GLFW_KEY_CAPS_LOCK,
	eScrollLock =   GLFW_KEY_SCROLL_LOCK,
	eNumLock =      GLFW_KEY_NUM_LOCK,
	ePrintScreen =  GLFW_KEY_PRINT_SCREEN,
	ePause =        GLFW_KEY_PAUSE,
	eF1 =           GLFW_KEY_F1,
	eF2 =           GLFW_KEY_F2,
	eF3 =           GLFW_KEY_F3,
	eF4 =           GLFW_KEY_F4,
	eF5 =           GLFW_KEY_F5,
	eF6 =           GLFW_KEY_F6,
	eF7 =           GLFW_KEY_F7,
	eF8 =           GLFW_KEY_F8,
	eF9 =           GLFW_KEY_F9,
	eF10 =          GLFW_KEY_F10,
	eF11 =          GLFW_KEY_F11,
	eF12 =          GLFW_KEY_F12,
	eF13 =          GLFW_KEY_F13,
	eF14 =          GLFW_KEY_F14,
	eF15 =          GLFW_KEY_F15,
	eF16 =          GLFW_KEY_F16,
	eF17 =          GLFW_KEY_F17,
	eF18 =          GLFW_KEY_F18,
	eF19 =          GLFW_KEY_F19,
	eF20 =          GLFW_KEY_F20,
	eF21 =          GLFW_KEY_F21,
	eF22 =          GLFW_KEY_F22,
	eF23 =          GLFW_KEY_F23,
	eF24 =          GLFW_KEY_F24,
	eF25 =          GLFW_KEY_F25,
	eKP0 =          GLFW_KEY_KP_0,
	eKP1 =          GLFW_KEY_KP_1,
	eKP2 =          GLFW_KEY_KP_2,
	eKP3 =          GLFW_KEY_KP_3,
	eKP4 =          GLFW_KEY_KP_4,
	eKP5 =          GLFW_KEY_KP_5,
	eKP6 =          GLFW_KEY_KP_6,
	eKP7 =          GLFW_KEY_KP_7,
	eKP8 =          GLFW_KEY_KP_8,
	eKP9 =          GLFW_KEY_KP_9,
	eKPDecimal =    GLFW_KEY_KP_DECIMAL,
	eKPDivice =     GLFW_KEY_KP_DIVIDE,
	eKPMultiply =   GLFW_KEY_KP_MULTIPLY,
	eKPSubstract =  GLFW_KEY_KP_SUBTRACT,
	eKPAdd =        GLFW_KEY_KP_ADD,
	eKPEnter =      GLFW_KEY_KP_ENTER,
	eKPEqual =      GLFW_KEY_KP_EQUAL,
	eLeftShift =    GLFW_KEY_LEFT_SHIFT,
	eLeftControl =  GLFW_KEY_LEFT_CONTROL,
	eLeftAlt =      GLFW_KEY_LEFT_ALT,
	eLeftSuper =    GLFW_KEY_LEFT_SUPER,
	eRightShift =   GLFW_KEY_RIGHT_SHIFT,
	eRightControl = GLFW_KEY_RIGHT_CONTROL,
	eRightAlt =     GLFW_KEY_RIGHT_ALT,
	eRightSuper =   GLFW_KEY_RIGHT_SUPER,
	eMenu =         GLFW_KEY_MENU,

	eLast =         GLFW_KEY_LAST
};

enum class KeyModifierFlagBits {
	eShift =    GLFW_MOD_SHIFT,
	eControl =  GLFW_MOD_CONTROL,
	eAlt =      GLFW_MOD_ALT,
	eSuper =    GLFW_MOD_SUPER,
	eCapsLock = GLFW_MOD_CAPS_LOCK,
	eNumLock =  GLFW_MOD_NUM_LOCK
};

using KeyModifiersFlags = vk::Flags<KeyModifierFlagBits, int>;

class RenderWindow {
public:
	RenderWindow(int width, int height, std::string name);
	RenderWindow(const RenderWindow&) = delete;
	RenderWindow(RenderWindow&&) = delete;
	~RenderWindow();

	RenderWindow& operator=(const RenderWindow&) = delete;
	RenderWindow& operator=(RenderWindow&&) = delete;

	template<typename Dispatch>
	vk::UniqueHandle<vk::SurfaceKHR, Dispatch> createSurfaceUnique(vk::Instance instance, Dispatch d) {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
		vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
		surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
		return instance.createWin32SurfaceKHRUnique(surfaceCreateInfo, nullptr, d);
#elif defined(USE_LINUX_OPERATING_SYSTEM)
		vk::XlibSurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.dpy = glfwGetX11Display();
		surfaceCreateInfo.window = glfwGetX11Window(window);
		return instance.createXlibSurfaceKHRUnique(surfaceCreateInfo, nullptr, d);
#endif
	}
	template<typename Dispatch>
	vk::SurfaceKHR createSurface(vk::Instance instance, Dispatch d) {
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
		vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
		surfaceCreateInfo.hwnd = glfwGetWin32Window(window);
		return instance.createWin32SurfaceKHR(surfaceCreateInfo, nullptr, d);
#elif defined(USE_LINUX_OPERATING_SYSTEM)
		vk::XlibSurfaceCreateInfoKHR surfaceCreateInfo;
		surfaceCreateInfo.dpy = glfwGetX11Display();
		surfaceCreateInfo.window = glfwGetX11Window(window);
		return instance.createXlibSurfaceKHR(surfaceCreateInfo, nullptr, d);
#endif
	}

	int getWidth();
	int getHeight();
	std::vector<const char*> getRequiredExtensions();

	using keyCallbackFunction = std::function<void(Key key, int scancode, int action, KeyModifiersFlags modifiers)>;

	void close();
	bool isClosed();

	void setUserPointer(void* ptr);
	void setResizeCallback(GLFWwindowsizefun resizeFunction);
	void setKeyCallback(keyCallbackFunction keyFunction);

	void setTitle(const std::string&);

	void pollEvents();

private:

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	keyCallbackFunction keyFun;


	GLFWwindow * window;
	int width;
	int height;
};

#endif
