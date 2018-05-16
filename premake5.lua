-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "Release" }

	local sourceDir = "src/"
	local LibDir = "Libraries/"

	location ( "build/" .. _ACTION )
	targetdir "build/bin/%{cfg.platform}/%{cfg.buildcfg}"
	objdir "build/obj/"
	debugdir "./"
	
	language "C++"
	cppdialect "C++11"
	architecture "x86_64"
	
	flags { "MultiProcessorCompile" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		targetsuffix "-d"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "system:Windows"
		defines { "USE_WINDOWS_OPERATING_SYSTEM" }
		libdirs { LibDir .. "Lib/*" }

	filter "system:Linux"
		defines { "USE_LINUX_OPERATING_SYSTEM" }
		links { "dl" }

	filter {}

	vpaths {
		["Headers"] = { "**.hpp", "**.inl" },
		["Sources"] = { "**.cpp" },
		["Shaders"] = { "**.vert", "**.frag" }
	}

	defines { "VK_NO_PROTOTYPES" }

	include( "Libraries/findVulkan.lua" )

	includeVulkan()

project "VkHelloTriangle"
	kind "StaticLib"

	includedirs { LibDir .. "Include" }

	files { sourceDir .. "**.hpp", sourceDir .. "**.inl", sourceDir .. "**.cpp" }


project "printVulkanInfos"
	kind "ConsoleApp"
	includedirs { LibDir .. "Include", sourceDir }
	files { "examples/printCapabilities.cpp" }

	links "VkHelloTriangle"

project "vulkan-tutorial"
	kind "ConsoleApp"
	includedirs { LibDir .. "Include", sourceDir  }
	files { "examples/vulkan-tutorial/*", "ressources/shaders/*.frag", "ressources/shaders/*.vert" }

	filter "system:Windows"
		links "glfw3"

	filter { "system:Linux" , "action:gmake2" }
		buildoptions { "`pkg-config --cflags glfw3`" }
		linkoptions { "`pkg-config --static --libs glfw3`" }

	filter {}

	links { "VkHelloTriangle" }
