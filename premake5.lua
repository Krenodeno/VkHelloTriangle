-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug32", "Release32", "Debug64", "Release64" }
	platforms { "Win32", "Linux" }

project "VkHelloTriangle"
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"

	-- Includes
	includedirs { 
		"Librairies/Include"
	}
	-- Librairies
	libdirs {
		"Librairies/Lib"
	}

	files { "Hello Triangle/**.hpp", "Hello Triangle/**.inl", "Hello Triangle/**.cpp" }

	filter "configurations:Debug*"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release*"
		defines { "NDEBUG" }
		optimize "On"

	filter "configurations:*32"
		architecture "x86"

	filter "configurations:*64"
		architecture "x86_64"

	filter "platforms:Win32"
		system "windows"

	filter "platforms:Linux"
		system "linux"