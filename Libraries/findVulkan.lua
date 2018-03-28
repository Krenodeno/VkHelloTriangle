-- findVulkan.lua

-- Lunarg's Vulkan SDK set Windows environment variables, so use it
envVulkanSDKPath = os.getenv("VK_SDK_PATH")
print(envVulkanSDKPath)
vulkanSDKPath = envVulkanSDKPath:gsub("\\", "/")

if (vulkanSDKPath) then
	includedirs { vulkanSDKPath .. "" }
end
