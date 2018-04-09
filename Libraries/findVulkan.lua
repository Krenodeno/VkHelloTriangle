-- findVulkan.lua
function includeVulkan()
	-- LunarG's Vulkan SDK set an environment variable, so use it
	local envVulkanSDKPath = os.getenv("VULKAN_SDK")
	if (envVulkanSDKPath) then
		-- get rid of the counter slash of Windows (does nothing on Linux)
		includedirs { envVulkanSDKPath:gsub("\\", "/") .. "Include" }
	elseif (os.findlib("vulkan")) then
		-- use the well known include paths
	else
		print ("Vulkan SDK not found !")
	end
end