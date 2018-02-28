-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "Release" }
	platforms { "x64", "x86" }
	
	local sourceDir = "src/"
	local LibDir = "Libraries/"

	targetdir "build/bin/%{cfg.buildcfg}"
	objdir "build/"

	cppdialect "C++11"

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


project "printCapabilities"
	kind "ConsoleApp"
	language "C++"

	includedirs { LibDir .. "Include", sourceDir }
	files { "examples/printCapabilities.cpp" }

	links "VkHelloTriangle"
	
project "ModelNoLighting"
	kind "ConsoleApp"
	language "C++"
	
	includedirs { LibDir .. "Include", sourceDir }
	files { "examples/ModelNoLighting/*" }
	
	links { "glfw3" }
	
	links { "VkHelloTriangle" }
	