-- findVulkan.lua
function includeVulkan()
	-- LunarG's Vulkan SDK set an environment variable, so use it
	local envVulkanSDKPath = os.getenv("VULKAN_SDK")
	
	filter "system:Windows"
		if (envVulkanSDKPath) then
			-- get rid of the counter slash
			local vulkanSDKPath = envVulkanSDKPath:gsub("\\", "/")
			print("Found Vulkan library : " .. vulkanSDKPath)
			includedirs { vulkanSDKPath .. "/Include" }
		else
			print("Vulkan SDK not found !")
		end
		
	filter "system:Linux"
		-- try to find the lib before searching for the LunarG SDK
		if (p = os.findlib("vulkan")) then
			print("Found Vulkan library : " .. p)
		elseif (envVulkanSDKPath) then
			print(("Found Vulkan library : " .. envVulkanSDKPath)
			includedirs { envVulkanSDKPath .. "/Include" }
		else
			print("Vulkan SDK not found !")
		end
	
	filter {}
end