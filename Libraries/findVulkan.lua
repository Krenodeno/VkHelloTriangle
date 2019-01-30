function getVulkanPath()
	-- LunarG's Vulkan SDK set an environment variable, so use it
	local envVulkanSDKPath = os.getenv("VULKAN_SDK")
	local vulkanPath = os.findlib("vulkan")
	if (envVulkanSDKPath) then
		return envVulkanSDKPath:gsub("\\", "/")
	elseif (vulkanPath) then
		print("Vulkan SDK found in system's libraries")
	else
		print ("Vulkan SDK not found !")
	end
end

-- findVulkan.lua
function includeVulkan()
	local libPath = getVulkanPath();
	if (libPath) then
		filter "system:Windows"
			-- get rid of the counter slash of Windows
			includedirs { libPath:gsub("\\", "/") .. "/Include" }
		filter "system:Linux"
			includedirs { libPath .. "/include" }
		filter {}
	end
end

function linkVulkan()
	local libPath = getVulkanPath();
	if (libPath) then
		filter "system:Windows"
			-- get rid of the counter slash of Windows
			libdirs { libPath:gsub("\\", "/") .. "/Lib" }
		filter "system:Linux"
			libdirs { libPath .. "/lib" }
		filter {}
	end
	filter "system:Windows"
		links { "vulkan-1" }
	filter "system:Linux"
		links { "vulkan" }
	filter {}
end
