-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "Release" }
	platforms { "x64", "x86" }
	
	local sourceDir = "src/"
	local LibDir = "Libraries/"

	location ( "build/" .. _ACTION )
	targetdir "build/bin/%{cfg.platform}/%{cfg.buildcfg}"
	objdir "build/obj/"
	debugdir "./"

	cppdialect "C++11"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		targetsuffix "-d"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"

	filter "system:Windows"
		defines { "VK_USE_PLATFORM_WIN32_KHR" }

	filter "system:Linux"
		defines { "VK_USE_PLATFORM_XCB_KHR", "VK_USE_PLATFORM_XLIB_KHR" }
		links { "dl" }

	filter { "system:Windows", "platforms:x86" }
		libdirs { LibDir .. "Lib32/*" }
	
	filter { "system:Windows", "platforms:x64" }
		libdirs { LibDir .. "Lib/*" }

	filter {}
	
	vpaths {
		["Headers"] = { "**.hpp", "**.inl" },
		["Sources"] = { "**.cpp" },
		["Shaders"] = { "**.vert", "**.frag" }
	}
	
	defines { "VK_NO_PROTOTYPES" }

project "VkHelloTriangle"
	kind "StaticLib"
	language "C++"

	-- Includes
	includedirs { 
		LibDir .. "Include"
	}

	files { sourceDir .. "**.hpp", sourceDir .. "**.inl", sourceDir .. "**.cpp" }

	filter {}


project "printVulkanInfos"
	kind "ConsoleApp"
	language "C++"

	includedirs { LibDir .. "Include", sourceDir }
	files { "examples/printCapabilities.cpp" }

	links "VkHelloTriangle"
	
project "vulkan-tutorial"
	kind "ConsoleApp"
	language "C++"
	
	includedirs { LibDir .. "Include", sourceDir }
	files { "examples/vulkan-tutorial/*" }
	
	links { "glfw3" }
	
	links { "VkHelloTriangle" }
	