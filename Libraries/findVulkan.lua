-- findVulkan.lua
function includeVulkan()
	-- LunarG's Vulkan SDK set a Windows environment variable, so use it
	local envVulkanSDKPath = os.getenv("VK_SDK_PATH")

	if (envVulkanSDKPath) then
		local vulkanSDKPath = envVulkanSDKPath:gsub("\\", "/")
		print("Found Vulkan librarie : " .. vulkanSDKPath)
		includedirs { vulkanSDKPath .. "/Include" }
	end
end