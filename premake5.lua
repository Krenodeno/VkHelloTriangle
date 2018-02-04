-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "Release" }
	platforms { "x64", "x86" }

project "VkHelloTriangle"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"

	-- Includes
	includedirs { 
		"Libraries/Include"
	}

	files { "Hello Triangle/**.hpp", "Hello Triangle/**.inl", "Hello Triangle/**.cpp" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"

	filter "system:Windows"
		defines "VK_USE_PLATFORM_WIN32_KHR"

	filter "system:Linux"
		defines { "VK_USE_PLATFORM_XCB_KHR", "VK_USE_PLATFORM_XLIB_KHR" }
		links { "glfw"}

	filter { "system:Windows", "platforms:x86" }
		libdirs { "Libraries/Lib32" }
	
	filter { "system:Windows", "platforms:x64" }
		libdirs { "Libraries/Lib" }
