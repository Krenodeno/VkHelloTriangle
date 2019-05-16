-- premake5.lua
workspace "VkHelloTriangle"
	configurations { "Debug", "DebugSAN", "Release" }

	local rootDir =   "../../"
	local sourceDir = rootDir .. "src/"
	local sampleDir = rootDir .. "examples/"
	local libDir =    rootDir .. "external/"
	local dataDir =   rootDir .. "data/"
	local buildDir =  rootDir .. "build/"

	location( buildDir )
	targetdir( buildDir .. "bin/%{cfg.platform}/%{cfg.buildcfg}/" )
	objdir( buildDir .. "obj/" )
	debugdir( rootDir )

	language "C++"
	cppdialect "C++17"
	architecture "x86_64"

	startproject "vulkan-tutorial"
	flags { "MultiProcessorCompile" }
	warnings "Extra"

	filter "configurations:Debug*"
		defines { "DEBUG" }
		symbols "On"
		optimize "Off"
		targetsuffix "-d"

--	filter { "toolset:not gcc", "toolset:not clang" }
--		removeconfigurations "*SAN"

	filter { "configurations:*SAN", "toolset:gcc or clang" }
		buildoptions { "-fno-omit-frame-pointer" }
		buildoptions { "-fsanitize=undefined,address" }
		linkoptions { "-fsanitize=undefined,address" }

	filter "configurations:Release*"
		defines { "NDEBUG" }
		symbols "Off"
		optimize "On"

	filter "configurations:DebugSAN"
		targetsuffix "-dsan"

	filter "system:Windows"
		defines {
			"USE_WINDOWS_OPERATING_SYSTEM",
			"VK_USE_PLATFORM_WIN32_KHR",
			"NOMINMAX"
		}
		libdirs({ libDir .. "lib/*" })


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
		["Shaders"] = { "**.vert", "**.frag", ".comp" }
	}

	include( "findVulkan.lua" )

	includeVulkan()

	linkVulkan()

project "CompileShaders"
	kind "Utility"
	files({ dataDir .. "shaders/*.comp", dataDir .. "shaders/*.frag", dataDir .. "shaders/*.vert"})

	local vulkanSDK = getVulkanPath()
	local glslCompiler = "glslangValidator"
	if (vulkanSDK) then
		glslCompiler = vulkanSDK .. '/bin/glslangValidator'
	end

	filter "files:**/shaders/*"
		buildcommands(glslCompiler .. ' -V -o "%{file.reldirectory}/%{file.name}.spv" "%{file.relpath}"')
		--buildinputs "%{file.reldirectory}/%{file.name}"
		buildoutputs "%{file.reldirectory}/%{file.name}.spv"


project "VkHelloTriangle"
	kind "StaticLib"

	includedirs({ libDir .. "include" })

	files({ sourceDir .. "**.hpp", sourceDir .. "**.inl", sourceDir .. "**.cpp" })


project "printVulkanInfos"
	kind "ConsoleApp"
	includedirs({ libDir .. "include", sourceDir })
	files( sampleDir .. "printCapabilities.cpp" )

	links "VkHelloTriangle"


project "vulkan-tutorial"
	kind "ConsoleApp"
	includedirs({ libDir .. "include", sourceDir })
	files({ sampleDir .. "vulkan-tutorial/*", dataDir .. "shaders/*.frag", dataDir .. "shaders/*.vert" })

	filter "system:Windows"
		links "glfw3"

	filter { "system:Linux" , "action:gmake2" }
		buildoptions { "`pkg-config --cflags glfw3`" }
		linkoptions  { "`pkg-config --static --libs glfw3`" }

	filter {}

	links { "VkHelloTriangle", "CompileShaders" }


project "compute-fractal"
	kind "ConsoleApp"
	includedirs({ libDir .. "include", sourceDir })
	files({ sampleDir .. "compute/*", dataDir .. "shaders/*.comp" })

	links { "CompileShaders" }
