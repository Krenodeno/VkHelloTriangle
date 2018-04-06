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
	
	flags { "MultiProcessorCompile" }

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
		defines { "USE_WINDOWS_OPERATING_SYSTEM" }

	filter "system:Linux"
		defines { "USE_LINUX_OPERATING_SYSTEM" }
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

	include( "Libraries/findVulkan.lua" )

project "VkHelloTriangle"
	kind "StaticLib"
	language "C++"

	includedirs { LibDir .. "Include" }
	includeVulkan()

	files { sourceDir .. "**.hpp", sourceDir .. "**.inl", sourceDir .. "**.cpp" }


project "printVulkanInfos"
	kind "ConsoleApp"
	language "C++"
	includedirs { LibDir .. "Include", sourceDir }
	includeVulkan()
	files { "examples/printCapabilities.cpp" }

	links "VkHelloTriangle"

project "vulkan-tutorial"
	kind "ConsoleApp"
	language "C++"
	includedirs { LibDir .. "Include", sourceDir  }
	includeVulkan()
	files { "examples/vulkan-tutorial/*", "ressources/shaders/*.frag", "ressources/shaders/*.vert" }

	filter "system:Windows"
		links "glfw3"

	filter { "system:Linux" , "action:gmake2" }
		buildoptions { "`pkg-config --cflags glfw3`" }
		linkoptions { "`pkg-config --static --libs glfw3`" }

	filter {}

	links { "VkHelloTriangle" }
