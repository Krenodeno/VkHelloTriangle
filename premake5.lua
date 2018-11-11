-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "DebugWSAN", "Release" }

	local sourceDir = "src/"
	local LibDir = "Libraries/"

	location ( "build/" .. _ACTION )
	targetdir "build/bin/%{cfg.platform}/%{cfg.buildcfg}"
	objdir "build/obj/"
	debugdir "./"

	language "C++"
	cppdialect "C++17"
	architecture "x86_64"

	startproject "vulkan-tutorial"
	flags { "MultiProcessorCompile" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		targetsuffix "-d"

	filter "configurations:DebugWSAN"
		defines { "DEBUG" }
		symbols "On"
		targetsuffix "-dsan"
		buildoptions { "-fno-omit-frame-pointer" }
		buildoptions { "-fsanitize=undefined,address" }
		linkoptions { "-fsanitize=undefined,address" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

	filter "system:Windows"
		defines {
			"USE_WINDOWS_OPERATING_SYSTEM",
			"VK_USE_PLATFORM_WIN32_KHR",
			"NOMINMAX"
		}
		libdirs { LibDir .. "Lib/*" }


	filter "system:Linux"
		defines {
			"USE_LINUX_OPERATING_SYSTEM",
			"VK_USE_PLATFORM_WAYLAND_KHR",
			"VK_USE_PLATFORM_XCB_KHR",
			"VK_USE_PLATFORM_XLIB_KHR"
		}
		links { "dl" }

	filter {}

	vpaths {
		["Headers"] = { "**.hpp", "**.inl" },
		["Sources"] = { "**.cpp" },
		["Shaders"] = { "**.vert", "**.frag" }
	}

	include( "Libraries/findVulkan.lua" )

	includeVulkan()

	linkVulkan()

project "VkHelloTriangle"
	kind "StaticLib"

	includedirs { LibDir .. "Include" }

	files { sourceDir .. "**.hpp", sourceDir .. "**.inl", sourceDir .. "**.cpp" }


project "CompileShaders"
	kind "Utility"

	files { "ressources/shaders/**.vert" , "ressources/shaders/**.frag" }

	-- A message to display while this build step is running (optional)
	buildmessage 'Compiling %{file.relpath} to SPIR-V'

	local glslangValidatorPath = getVulkanPath()
	if (glslangValidatorPath) then
		-- One or more commands to run (required)
		buildcommands({ glslangValidatorPath .. ' -V "%{file.relpath}"' })
	else
		-- One or more commands to run (required)
		buildcommands{ 'glslangValidator -V "%{file.relpath}"' }
	end
	filter { "files:**.vert" }

		-- One or more outputs resulting from the build (required)
		buildoutputs { 'vert.spv' }

	filter { "files:**.frag" }

		-- One or more outputs resulting from the build (required)
		buildoutputs { 'frag.spv' }

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

	links { "VkHelloTriangle" , "CompileShaders" }
