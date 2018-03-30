-- findVulkan.lua
function includeVulkan()
	filter "system:Windows"
		-- LunarG's Vulkan SDK set a Windows environment variable, so use it
		local envVulkanSDKPath = os.getenv("VK_SDK_PATH")

		if (envVulkanSDKPath) then
			local vulkanSDKPath = envVulkanSDKPath:gsub("\\", "/")
			print("Found Vulkan librarie : " .. vulkanSDKPath)
			includedirs { vulkanSDKPath .. "/Include" }
		else
			print("Vulkan SDK not found !")
		end
	filter {}
end