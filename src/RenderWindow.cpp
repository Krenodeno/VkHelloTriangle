#include "RenderWindow.hpp"

RenderWindow::RenderWindow(int width, int height, std::string name) : width(width), height(height) {
	if (!glfwInit())
		throw std::runtime_error("Can't initalize GLFW !");
	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "GLFW Error[" << error << "]: " << description << "\n";
	});
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
	if (window == nullptr)
		throw std::runtime_error("Can't create Window !");

	glfwSetWindowUserPointer(window, this);
	glfwSetWindowSizeCallback(window, &resizeCallback);
	glfwSetKeyCallback(window, &keyCallback);
}

RenderWindow::~RenderWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

int RenderWindow::getWidth() {
	glfwGetWindowSize(window, &width, &height);
	return width;
}

int RenderWindow::getHeight() {
	glfwGetWindowSize(window, &width, &height);
	return height;
}

std::vector<const char*> RenderWindow::getRequiredExtensions() {
	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#if defined(USE_WINDOWS_OPERATING_SYSTEM)
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(USE_LINUX_OPERATING_SYSTEM)
	extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
	extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#endif
	return extensions;
}


void RenderWindow::close() {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool RenderWindow::isClosed() {
	return glfwWindowShouldClose(window);
}

void RenderWindow::setUserPointer(void * ptr) {
	glfwSetWindowUserPointer(window, ptr);
}

void RenderWindow::setResizeCallback(resizeCallbackFunction resizeFunction) {
	resizeFun = resizeFunction;
}

void RenderWindow::setKeyCallback(keyCallbackFunction keyFunction) {
	keyFun = keyFunction;
}

void RenderWindow::setTitle(const std::string& title) {
	glfwSetWindowTitle(window, title.c_str());
}

void RenderWindow::pollEvents() {
	glfwPollEvents();
}

void RenderWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
	RenderWindow* windowWrapper = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
	assert(windowWrapper);
	windowWrapper->resizeFun(width, height);
}

void RenderWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	RenderWindow* windowWrapper = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));

	assert(windowWrapper);

	windowWrapper->keyFun(static_cast<Key>(key), scancode, action, static_cast<KeyModifiersFlags>(mods));
}
