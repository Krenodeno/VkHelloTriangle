-- findVulkan.lua
function includeVulkan()
	-- LunarG's Vulkan SDK set an environment variable, so use it
	local envVulkanSDKPath = os.getenv("VULKAN_SDK")
	if (envVulkanSDKPath) then
		filter "system:Windows"
			-- get rid of the counter slash of Windows
			includedirs { envVulkanSDKPath:gsub("\\", "/") .. "/Include" }
		filter "system:Linux"
			includedirs { envVulkanSDKPath .. "/include" }
		filter {}
	elseif (os.findlib("vulkan")) then
		-- use the well known include paths
	else
		print ("Vulkan SDK not found !")
	end
end

function linkVulkan()
-- LunarG's Vulkan SDK set an environment variable, so use it
	local envVulkanSDKPath = os.getenv("VULKAN_SDK")
	if (envVulkanSDKPath) then
		filter "system:Windows"
			-- get rid of the counter slash of Windows
			libdirs { envVulkanSDKPath:gsub("\\", "/") .. "/Lib" }
			links { "vulkan-1" }
		filter "system:Linux"
			libdirs { envVulkanSDKPath .. "/lib" }
			links { "vulkan" }
		filter {}
	elseif (os.findlib("vulkan")) then
		-- use the well known include paths
		filter "system:Windows"
			links { "vulkan-1" }
		filter "system:Linux"
			links { "vulkan" }
		filter {}
	else
		print ("Vulkan SDK not found !")
	end
end
