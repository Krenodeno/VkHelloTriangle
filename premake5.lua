-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "Release" }
	platforms { "x64", "x86" }
	
	local sourceDir = "src/"
	local LibDir = "Libraries/"
	
	targetdir "build/bin/%{cfg.buildcfg}"
	objdir "build/"

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
		
	filter {}
	
	vpaths {
		["Headers"] = { "**.hpp", "**.inl" },
		["Sources"] = "**.cpp"
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
	
	--links { "glfw3" }

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


project "printCapabilities"
	kind "ConsoleApp"
	language "C++"
	
	includedirs { LibDir .. "Include", sourceDir }
	files { "examples/printCapabilities.cpp" }
	
	links "VkHelloTriangle"