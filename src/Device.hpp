#ifndef VK_DEVICE_HPP
#define VK_DEVICE_HPP

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>


/*
 * (([a-z][A-z]+)\((([A-z*&:<> ]+) ([a-z][A-z]*)( = [A-z:()]+)?),(([A-z*&:<> ]+) ([a-z][A-z]*)( = [A-z:()]+)?) \) const);
 * $1 {\n		handle.$2($5, $9, dispatch);\n	}
*/

class Device {

public:

	Device();
	~Device();

	Device(const Device&) = delete;
	Device(Device&&) = default;

	operator vk::Device() { return handle; }

#ifdef VK_USE_PLATFORM_WIN32_KHR
#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::Result acquireFullScreenExclusiveModeEXT( vk::SwapchainKHR swapchain ) const {
		handle.acquireFullScreenExclusiveModeEXT(swapchain, dispatch);
	}
#else

	vk::ResultValueType<void>::type acquireFullScreenExclusiveModeEXT( vk::SwapchainKHR swapchain ) const {
		handle.acquireFullScreenExclusiveModeEXT(swapchain, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/


	vk::Result acquireNextImage2KHR( const vk::AcquireNextImageInfoKHR* pAcquireInfo, uint32_t* pImageIndex ) const {
		handle.acquireNextImage2KHR(pAcquireInfo, pImageIndex, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValue<uint32_t> acquireNextImage2KHR( const vk::AcquireNextImageInfoKHR & acquireInfo ) const {
		handle.acquireNextImage2KHR(acquireInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result acquireNextImageKHR( vk::SwapchainKHR swapchain, uint64_t timeout, vk::Semaphore semaphore, vk::Fence fence, uint32_t* pImageIndex ) const {
		handle.acquireNextImageKHR(swapchain, timeout, semaphore, fence, pImageIndex, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValue<uint32_t> acquireNextImageKHR( vk::SwapchainKHR swapchain, uint64_t timeout, vk::Semaphore semaphore, vk::Fence fence ) const {
		handle.acquireNextImageKHR(swapchain, timeout, semaphore, fence, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result acquirePerformanceConfigurationINTEL( const vk::PerformanceConfigurationAcquireInfoINTEL* pAcquireInfo, vk::PerformanceConfigurationINTEL* pConfiguration ) const {
		handle.acquirePerformanceConfigurationINTEL(pAcquireInfo, pConfiguration, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType<vk::PerformanceConfigurationINTEL>::type acquirePerformanceConfigurationINTEL( const vk::PerformanceConfigurationAcquireInfoINTEL & acquireInfo ) const {
		handle.acquirePerformanceConfigurationINTEL(acquireInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result allocateCommandBuffers( const vk::CommandBufferAllocateInfo* pAllocateInfo, vk::CommandBuffer* pCommandBuffers ) const {
		handle.allocateCommandBuffers(pAllocateInfo, pCommandBuffers, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::CommandBuffer>>
	typename vk::ResultValueType<std::vector<vk::CommandBuffer,Allocator>>::type allocateCommandBuffers( const vk::CommandBufferAllocateInfo & allocateInfo ) const {
		handle.allocateCommandBuffers(allocateInfo, dispatch);
	}
	template<typename Allocator = std::allocator<vk::CommandBuffer>>
	typename vk::ResultValueType<std::vector<vk::CommandBuffer,Allocator>>::type allocateCommandBuffers( const vk::CommandBufferAllocateInfo & allocateInfo, Allocator const& vectorAllocator ) const {
		handle.allocateCommandBuffers(allocateInfo, vectorAllocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template<typename Allocator = std::allocator<vk::UniqueCommandBuffer>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::CommandBuffer,vk::DispatchLoaderDynamic>,Allocator>>::type allocateCommandBuffersUnique( const vk::CommandBufferAllocateInfo & allocateInfo ) const {
		handle.allocateCommandBuffersUnique(allocateInfo, dispatch);
	}
	template<typename Allocator = std::allocator<vk::UniqueCommandBuffer>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::CommandBuffer,vk::DispatchLoaderDynamic>,Allocator>>::type allocateCommandBuffersUnique( const vk::CommandBufferAllocateInfo & allocateInfo, Allocator const& vectorAllocator ) const {
		handle.allocateCommandBuffersUnique(allocateInfo, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result allocateDescriptorSets( const vk::DescriptorSetAllocateInfo* pAllocateInfo, vk::DescriptorSet* pDescriptorSets ) const {
		handle.allocateDescriptorSets(pAllocateInfo, pDescriptorSets, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::DescriptorSet>>
	typename vk::ResultValueType<std::vector<vk::DescriptorSet,Allocator>>::type allocateDescriptorSets( const vk::DescriptorSetAllocateInfo & allocateInfo ) const {
		handle.allocateDescriptorSets(allocateInfo, dispatch);
	}
	template<typename Allocator = std::allocator<vk::DescriptorSet>>
	typename vk::ResultValueType<std::vector<vk::DescriptorSet,Allocator>>::type allocateDescriptorSets( const vk::DescriptorSetAllocateInfo & allocateInfo, Allocator const& vectorAllocator ) const {
		handle.allocateDescriptorSets(allocateInfo, vectorAllocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template<typename Allocator = std::allocator<vk::UniqueDescriptorSet>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::DescriptorSet,vk::DispatchLoaderDynamic>,Allocator>>::type allocateDescriptorSetsUnique( const vk::DescriptorSetAllocateInfo & allocateInfo ) const {
		handle.allocateDescriptorSetsUnique(allocateInfo, dispatch);
	}
	template<typename Allocator = std::allocator<vk::UniqueDescriptorSet>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::DescriptorSet,vk::DispatchLoaderDynamic>,Allocator>>::type allocateDescriptorSetsUnique( const vk::DescriptorSetAllocateInfo & allocateInfo, Allocator const& vectorAllocator ) const {
		handle.allocateDescriptorSetsUnique(allocateInfo, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result allocateMemory( const vk::MemoryAllocateInfo* pAllocateInfo, const vk::AllocationCallbacks* pAllocator, vk::DeviceMemory* pMemory ) const {
		handle.allocateMemory(pAllocateInfo, pAllocator, pMemory, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType<vk::DeviceMemory>::type allocateMemory( const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.allocateMemory(allocateInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE

	typename vk::ResultValueType<vk::UniqueHandle<vk::DeviceMemory,vk::DispatchLoaderDynamic>>::type allocateMemoryUnique( const vk::MemoryAllocateInfo & allocateInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.allocateMemoryUnique(allocateInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


	vk::Result bindAccelerationStructureMemoryNV( uint32_t bindInfoCount, const vk::BindAccelerationStructureMemoryInfoNV* pBindInfos ) const {
		handle.bindAccelerationStructureMemoryNV(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE

	vk::ResultValueType<void>::type bindAccelerationStructureMemoryNV( vk::ArrayProxy<const vk::BindAccelerationStructureMemoryInfoNV> bindInfos ) const {
		handle.bindAccelerationStructureMemoryNV(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result bindBufferMemory( vk::Buffer buffer, vk::DeviceMemory memory, vk::DeviceSize memoryOffset ) const {
		handle.bindBufferMemory(buffer, memory, memoryOffset, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type bindBufferMemory( vk::Buffer buffer, vk::DeviceMemory memory, vk::DeviceSize memoryOffset ) const {
		handle.bindBufferMemory(buffer, memory, memoryOffset, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result bindBufferMemory2( uint32_t bindInfoCount, const vk::BindBufferMemoryInfo* pBindInfos ) const {
		handle.bindBufferMemory2(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type bindBufferMemory2( vk::ArrayProxy<const vk::BindBufferMemoryInfo> bindInfos ) const {
		handle.bindBufferMemory2(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result bindBufferMemory2KHR( uint32_t bindInfoCount, const vk::BindBufferMemoryInfo* pBindInfos ) const {
		handle.bindBufferMemory2KHR(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type bindBufferMemory2KHR( vk::ArrayProxy<const vk::BindBufferMemoryInfo> bindInfos ) const {
		handle.bindBufferMemory2KHR(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result bindImageMemory( vk::Image image, vk::DeviceMemory memory, vk::DeviceSize memoryOffset ) const {
		handle.bindImageMemory(image, memory, memoryOffset, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type bindImageMemory( vk::Image image, vk::DeviceMemory memory, vk::DeviceSize memoryOffset ) const {
		handle.bindImageMemory(image, memory, memoryOffset, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result bindImageMemory2( uint32_t bindInfoCount, const vk::BindImageMemoryInfo* pBindInfos ) const {
		handle.bindImageMemory2(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type bindImageMemory2( vk::ArrayProxy<const vk::BindImageMemoryInfo> bindInfos ) const {
		handle.bindImageMemory2(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result bindImageMemory2KHR( uint32_t bindInfoCount, const vk::BindImageMemoryInfo* pBindInfos ) const {
		handle.bindImageMemory2KHR(bindInfoCount, pBindInfos, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type bindImageMemory2KHR( vk::ArrayProxy<const vk::BindImageMemoryInfo> bindInfos ) const {
		handle.bindImageMemory2KHR(bindInfos, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result compileDeferredNV( vk::Pipeline pipeline, uint32_t shader ) const {
		handle.compileDeferredNV(pipeline, shader, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type compileDeferredNV( vk::Pipeline pipeline, uint32_t shader ) const {
		handle.compileDeferredNV(pipeline, shader, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createAccelerationStructureNV( const vk::AccelerationStructureCreateInfoNV* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::AccelerationStructureNV* pAccelerationStructure ) const {
		handle.createAccelerationStructureNV(pCreateInfo, pAllocator, pAccelerationStructure, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::AccelerationStructureNV>::type createAccelerationStructureNV( const vk::AccelerationStructureCreateInfoNV & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createAccelerationStructureNV(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::AccelerationStructureNV,vk::DispatchLoaderDynamic>>::type createAccelerationStructureNVUnique( const vk::AccelerationStructureCreateInfoNV & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createAccelerationStructureNVUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createBuffer( const vk::BufferCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Buffer* pBuffer ) const {
		handle.createBuffer(pCreateInfo, pAllocator, pBuffer, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Buffer>::type createBuffer( const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createBuffer(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Buffer,vk::DispatchLoaderDynamic>>::type createBufferUnique( const vk::BufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createBufferUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createBufferView( const vk::BufferViewCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::BufferView* pView ) const {
		handle.createBufferView(pCreateInfo, pAllocator, pView, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::BufferView>::type createBufferView( const vk::BufferViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createBufferView(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::BufferView,vk::DispatchLoaderDynamic>>::type createBufferViewUnique( const vk::BufferViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createBufferViewUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createCommandPool( const vk::CommandPoolCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::CommandPool* pCommandPool ) const {
		handle.createCommandPool(pCreateInfo, pAllocator, pCommandPool, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::CommandPool>::type createCommandPool( const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createCommandPool(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::CommandPool,vk::DispatchLoaderDynamic>>::type createCommandPoolUnique( const vk::CommandPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createCommandPoolUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createComputePipelines( vk::PipelineCache pipelineCache, uint32_t createInfoCount, const vk::ComputePipelineCreateInfo* pCreateInfos, const vk::AllocationCallbacks* pAllocator, vk::Pipeline* pPipelines ) const {
		handle.createComputePipelines(pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::Pipeline>>
	typename vk::ResultValueType<std::vector<vk::Pipeline,Allocator>>::type createComputePipelines( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::ComputePipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createComputePipelines(pipelineCache, createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::Pipeline>>
	typename vk::ResultValueType<std::vector<vk::Pipeline,Allocator>>::type createComputePipelines( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::ComputePipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createComputePipelines(pipelineCache, createInfos, allocator, vectorAllocator, dispatch);
	}
 
	vk::ResultValueType<vk::Pipeline>::type createComputePipeline( vk::PipelineCache pipelineCache, const vk::ComputePipelineCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createComputePipeline(pipelineCache, createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template<typename Allocator = std::allocator<vk::UniquePipeline>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>,Allocator>>::type createComputePipelinesUnique( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::ComputePipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createComputePipelinesUnique(pipelineCache, createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::UniquePipeline>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>,Allocator>>::type createComputePipelinesUnique( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::ComputePipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createComputePipelinesUnique(pipelineCache, createInfos, allocator, vectorAllocator, dispatch);
	}
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>>::type createComputePipelineUnique( vk::PipelineCache pipelineCache, const vk::ComputePipelineCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createComputePipelineUnique(pipelineCache, createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createDescriptorPool( const vk::DescriptorPoolCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DescriptorPool* pDescriptorPool ) const {
		handle.createDescriptorPool(pCreateInfo, pAllocator, pDescriptorPool, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DescriptorPool>::type createDescriptorPool( const vk::DescriptorPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorPool(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::DescriptorPool,vk::DispatchLoaderDynamic>>::type createDescriptorPoolUnique( const vk::DescriptorPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorPoolUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createDescriptorSetLayout( const vk::DescriptorSetLayoutCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DescriptorSetLayout* pSetLayout ) const {
		handle.createDescriptorSetLayout(pCreateInfo, pAllocator, pSetLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DescriptorSetLayout>::type createDescriptorSetLayout( const vk::DescriptorSetLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorSetLayout(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::DescriptorSetLayout,vk::DispatchLoaderDynamic>>::type createDescriptorSetLayoutUnique( const vk::DescriptorSetLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorSetLayoutUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createDescriptorUpdateTemplate( const vk::DescriptorUpdateTemplateCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DescriptorUpdateTemplate* pDescriptorUpdateTemplate ) const {
		handle.createDescriptorUpdateTemplate(pCreateInfo, pAllocator, pDescriptorUpdateTemplate, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DescriptorUpdateTemplate>::type createDescriptorUpdateTemplate( const vk::DescriptorUpdateTemplateCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorUpdateTemplate(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::DescriptorUpdateTemplate,vk::DispatchLoaderDynamic>>::type createDescriptorUpdateTemplateUnique( const vk::DescriptorUpdateTemplateCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorUpdateTemplateUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createDescriptorUpdateTemplateKHR( const vk::DescriptorUpdateTemplateCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::DescriptorUpdateTemplate* pDescriptorUpdateTemplate ) const {
		handle.createDescriptorUpdateTemplateKHR(pCreateInfo, pAllocator, pDescriptorUpdateTemplate, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DescriptorUpdateTemplate>::type createDescriptorUpdateTemplateKHR( const vk::DescriptorUpdateTemplateCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorUpdateTemplateKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::DescriptorUpdateTemplate,vk::DispatchLoaderDynamic>>::type createDescriptorUpdateTemplateKHRUnique( const vk::DescriptorUpdateTemplateCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createDescriptorUpdateTemplateKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createEvent( const vk::EventCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Event* pEvent ) const {
		handle.createEvent(pCreateInfo, pAllocator, pEvent, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Event>::type createEvent( const vk::EventCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createEvent(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Event,vk::DispatchLoaderDynamic>>::type createEventUnique( const vk::EventCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createEventUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createFence( const vk::FenceCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Fence* pFence ) const {
		handle.createFence(pCreateInfo, pAllocator, pFence, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Fence>::type createFence( const vk::FenceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createFence(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Fence,vk::DispatchLoaderDynamic>>::type createFenceUnique( const vk::FenceCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createFenceUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createFramebuffer( const vk::FramebufferCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Framebuffer* pFramebuffer ) const {
		handle.createFramebuffer(pCreateInfo, pAllocator, pFramebuffer, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Framebuffer>::type createFramebuffer( const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createFramebuffer(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Framebuffer,vk::DispatchLoaderDynamic>>::type createFramebufferUnique( const vk::FramebufferCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createFramebufferUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createGraphicsPipelines( vk::PipelineCache pipelineCache, uint32_t createInfoCount, const vk::GraphicsPipelineCreateInfo* pCreateInfos, const vk::AllocationCallbacks* pAllocator, vk::Pipeline* pPipelines ) const {
		handle.createGraphicsPipelines(pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::Pipeline>>
	typename vk::ResultValueType<std::vector<vk::Pipeline,Allocator>>::type createGraphicsPipelines( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createGraphicsPipelines(pipelineCache, createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::Pipeline>>
	typename vk::ResultValueType<std::vector<vk::Pipeline,Allocator>>::type createGraphicsPipelines( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createGraphicsPipelines(pipelineCache, createInfos, allocator, vectorAllocator, dispatch);
	}
 
	vk::ResultValueType<vk::Pipeline>::type createGraphicsPipeline( vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createGraphicsPipeline(pipelineCache, createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template<typename Allocator = std::allocator<vk::UniquePipeline>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>,Allocator>>::type createGraphicsPipelinesUnique( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createGraphicsPipelinesUnique(pipelineCache, createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::UniquePipeline>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>,Allocator>>::type createGraphicsPipelinesUnique( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::GraphicsPipelineCreateInfo> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createGraphicsPipelinesUnique(pipelineCache, createInfos, allocator, vectorAllocator, dispatch);
	}
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>>::type createGraphicsPipelineUnique( vk::PipelineCache pipelineCache, const vk::GraphicsPipelineCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createGraphicsPipelineUnique(pipelineCache, createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createImage( const vk::ImageCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Image* pImage ) const {
		handle.createImage(pCreateInfo, pAllocator, pImage, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Image>::type createImage( const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createImage(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Image,vk::DispatchLoaderDynamic>>::type createImageUnique( const vk::ImageCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createImageUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createImageView( const vk::ImageViewCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::ImageView* pView ) const {
		handle.createImageView(pCreateInfo, pAllocator, pView, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::ImageView>::type createImageView( const vk::ImageViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createImageView(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::ImageView,vk::DispatchLoaderDynamic>>::type createImageViewUnique( const vk::ImageViewCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createImageViewUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createIndirectCommandsLayoutNVX( const vk::IndirectCommandsLayoutCreateInfoNVX* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::IndirectCommandsLayoutNVX* pIndirectCommandsLayout ) const {
		handle.createIndirectCommandsLayoutNVX(pCreateInfo, pAllocator, pIndirectCommandsLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::IndirectCommandsLayoutNVX>::type createIndirectCommandsLayoutNVX( const vk::IndirectCommandsLayoutCreateInfoNVX & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createIndirectCommandsLayoutNVX(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::IndirectCommandsLayoutNVX,vk::DispatchLoaderDynamic>>::type createIndirectCommandsLayoutNVXUnique( const vk::IndirectCommandsLayoutCreateInfoNVX & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createIndirectCommandsLayoutNVXUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createObjectTableNVX( const vk::ObjectTableCreateInfoNVX* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::ObjectTableNVX* pObjectTable ) const {
		handle.createObjectTableNVX(pCreateInfo, pAllocator, pObjectTable, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::ObjectTableNVX>::type createObjectTableNVX( const vk::ObjectTableCreateInfoNVX & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createObjectTableNVX(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::ObjectTableNVX,vk::DispatchLoaderDynamic>>::type createObjectTableNVXUnique( const vk::ObjectTableCreateInfoNVX & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createObjectTableNVXUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createPipelineCache( const vk::PipelineCacheCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::PipelineCache* pPipelineCache ) const {
		handle.createPipelineCache(pCreateInfo, pAllocator, pPipelineCache, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::PipelineCache>::type createPipelineCache( const vk::PipelineCacheCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createPipelineCache(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::PipelineCache,vk::DispatchLoaderDynamic>>::type createPipelineCacheUnique( const vk::PipelineCacheCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createPipelineCacheUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createPipelineLayout( const vk::PipelineLayoutCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::PipelineLayout* pPipelineLayout ) const {
		handle.createPipelineLayout(pCreateInfo, pAllocator, pPipelineLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::PipelineLayout>::type createPipelineLayout( const vk::PipelineLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createPipelineLayout(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::PipelineLayout,vk::DispatchLoaderDynamic>>::type createPipelineLayoutUnique( const vk::PipelineLayoutCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createPipelineLayoutUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createQueryPool( const vk::QueryPoolCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::QueryPool* pQueryPool ) const {
		handle.createQueryPool(pCreateInfo, pAllocator, pQueryPool, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::QueryPool>::type createQueryPool( const vk::QueryPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createQueryPool(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::QueryPool,vk::DispatchLoaderDynamic>>::type createQueryPoolUnique( const vk::QueryPoolCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createQueryPoolUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createRayTracingPipelinesNV( vk::PipelineCache pipelineCache, uint32_t createInfoCount, const vk::RayTracingPipelineCreateInfoNV* pCreateInfos, const vk::AllocationCallbacks* pAllocator, vk::Pipeline* pPipelines ) const {
		handle.createRayTracingPipelinesNV(pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::Pipeline>>
	typename vk::ResultValueType<std::vector<vk::Pipeline,Allocator>>::type createRayTracingPipelinesNV( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRayTracingPipelinesNV(pipelineCache, createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::Pipeline>>
	typename vk::ResultValueType<std::vector<vk::Pipeline,Allocator>>::type createRayTracingPipelinesNV( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createRayTracingPipelinesNV(pipelineCache, createInfos, allocator, vectorAllocator, dispatch);
	}
 
	vk::ResultValueType<vk::Pipeline>::type createRayTracingPipelineNV( vk::PipelineCache pipelineCache, const vk::RayTracingPipelineCreateInfoNV & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRayTracingPipelineNV(pipelineCache, createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template<typename Allocator = std::allocator<vk::UniquePipeline>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>,Allocator>>::type createRayTracingPipelinesNVUnique( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRayTracingPipelinesNVUnique(pipelineCache, createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::UniquePipeline>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>,Allocator>>::type createRayTracingPipelinesNVUnique( vk::PipelineCache pipelineCache, vk::ArrayProxy<const vk::RayTracingPipelineCreateInfoNV> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createRayTracingPipelinesNVUnique(pipelineCache, createInfos, allocator, vectorAllocator, dispatch);
	}
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Pipeline,vk::DispatchLoaderDynamic>>::type createRayTracingPipelineNVUnique( vk::PipelineCache pipelineCache, const vk::RayTracingPipelineCreateInfoNV & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRayTracingPipelineNVUnique(pipelineCache, createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createRenderPass( const vk::RenderPassCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::RenderPass* pRenderPass ) const {
		handle.createRenderPass(pCreateInfo, pAllocator, pRenderPass, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::RenderPass>::type createRenderPass( const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRenderPass(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::RenderPass,vk::DispatchLoaderDynamic>>::type createRenderPassUnique( const vk::RenderPassCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRenderPassUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createRenderPass2KHR( const vk::RenderPassCreateInfo2KHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::RenderPass* pRenderPass ) const {
		handle.createRenderPass2KHR(pCreateInfo, pAllocator, pRenderPass, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::RenderPass>::type createRenderPass2KHR( const vk::RenderPassCreateInfo2KHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRenderPass2KHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::RenderPass,vk::DispatchLoaderDynamic>>::type createRenderPass2KHRUnique( const vk::RenderPassCreateInfo2KHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createRenderPass2KHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createSampler( const vk::SamplerCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Sampler* pSampler ) const {
		handle.createSampler(pCreateInfo, pAllocator, pSampler, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Sampler>::type createSampler( const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSampler(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Sampler,vk::DispatchLoaderDynamic>>::type createSamplerUnique( const vk::SamplerCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSamplerUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createSamplerYcbcrConversion( const vk::SamplerYcbcrConversionCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SamplerYcbcrConversion* pYcbcrConversion ) const {
		handle.createSamplerYcbcrConversion(pCreateInfo, pAllocator, pYcbcrConversion, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::SamplerYcbcrConversion>::type createSamplerYcbcrConversion( const vk::SamplerYcbcrConversionCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSamplerYcbcrConversion(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::SamplerYcbcrConversion,vk::DispatchLoaderDynamic>>::type createSamplerYcbcrConversionUnique( const vk::SamplerYcbcrConversionCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSamplerYcbcrConversionUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createSamplerYcbcrConversionKHR( const vk::SamplerYcbcrConversionCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SamplerYcbcrConversion* pYcbcrConversion ) const {
		handle.createSamplerYcbcrConversionKHR(pCreateInfo, pAllocator, pYcbcrConversion, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::SamplerYcbcrConversion>::type createSamplerYcbcrConversionKHR( const vk::SamplerYcbcrConversionCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSamplerYcbcrConversionKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::SamplerYcbcrConversion,vk::DispatchLoaderDynamic>>::type createSamplerYcbcrConversionKHRUnique( const vk::SamplerYcbcrConversionCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSamplerYcbcrConversionKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createSemaphore( const vk::SemaphoreCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::Semaphore* pSemaphore ) const {
		handle.createSemaphore(pCreateInfo, pAllocator, pSemaphore, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Semaphore>::type createSemaphore( const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSemaphore(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::Semaphore,vk::DispatchLoaderDynamic>>::type createSemaphoreUnique( const vk::SemaphoreCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSemaphoreUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createShaderModule( const vk::ShaderModuleCreateInfo* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::ShaderModule* pShaderModule ) const {
		handle.createShaderModule(pCreateInfo, pAllocator, pShaderModule, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::ShaderModule>::type createShaderModule( const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createShaderModule(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::ShaderModule,vk::DispatchLoaderDynamic>>::type createShaderModuleUnique( const vk::ShaderModuleCreateInfo & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createShaderModuleUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createSharedSwapchainsKHR( uint32_t swapchainCount, const vk::SwapchainCreateInfoKHR* pCreateInfos, const vk::AllocationCallbacks* pAllocator, vk::SwapchainKHR* pSwapchains ) const {
		handle.createSharedSwapchainsKHR(swapchainCount, pCreateInfos, pAllocator, pSwapchains, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator< vk::SwapchainKHR>>
	typename vk::ResultValueType<std::vector< vk::SwapchainKHR,Allocator>>::type createSharedSwapchainsKHR( vk::ArrayProxy<const vk::SwapchainCreateInfoKHR> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSharedSwapchainsKHR(createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator< vk::SwapchainKHR>>
	typename vk::ResultValueType<std::vector< vk::SwapchainKHR,Allocator>>::type createSharedSwapchainsKHR( vk::ArrayProxy<const vk::SwapchainCreateInfoKHR> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createSharedSwapchainsKHR(createInfos, allocator, vectorAllocator, dispatch);
	}
 
	vk::ResultValueType< vk::SwapchainKHR>::type createSharedSwapchainKHR( const vk::SwapchainCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSharedSwapchainKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
	template<typename Allocator = std::allocator<vk::UniqueSwapchainKHR>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle< vk::SwapchainKHR,vk::DispatchLoaderDynamic>,Allocator>>::type createSharedSwapchainsKHRUnique( vk::ArrayProxy<const vk::SwapchainCreateInfoKHR> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSharedSwapchainsKHRUnique(createInfos, allocator, dispatch);
	}
	template<typename Allocator = std::allocator<vk::UniqueSwapchainKHR>>
	typename vk::ResultValueType<std::vector<vk::UniqueHandle< vk::SwapchainKHR,vk::DispatchLoaderDynamic>,Allocator>>::type createSharedSwapchainsKHRUnique( vk::ArrayProxy<const vk::SwapchainCreateInfoKHR> createInfos, vk::Optional<const vk::AllocationCallbacks> allocator, Allocator const& vectorAllocator ) const {
		handle.createSharedSwapchainsKHRUnique(createInfos, allocator, vectorAllocator, dispatch);
	}
 
	typename vk::ResultValueType<vk::UniqueHandle< vk::SwapchainKHR,vk::DispatchLoaderDynamic>>::type createSharedSwapchainKHRUnique( const vk::SwapchainCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSharedSwapchainKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createSwapchainKHR( const vk::SwapchainCreateInfoKHR* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::SwapchainKHR* pSwapchain ) const {
		handle.createSwapchainKHR(pCreateInfo, pAllocator, pSwapchain, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType< vk::SwapchainKHR>::type createSwapchainKHR( const vk::SwapchainCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSwapchainKHR(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle< vk::SwapchainKHR,vk::DispatchLoaderDynamic>>::type createSwapchainKHRUnique( const vk::SwapchainCreateInfoKHR & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createSwapchainKHRUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result createValidationCacheEXT( const vk::ValidationCacheCreateInfoEXT* pCreateInfo, const vk::AllocationCallbacks* pAllocator, vk::ValidationCacheEXT* pValidationCache ) const {
		handle.createValidationCacheEXT(pCreateInfo, pAllocator, pValidationCache, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::ValidationCacheEXT>::type createValidationCacheEXT( const vk::ValidationCacheCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createValidationCacheEXT(createInfo, allocator, dispatch);
	}
#ifndef VULKAN_HPP_NO_SMART_HANDLE
 
	typename vk::ResultValueType<vk::UniqueHandle<vk::ValidationCacheEXT,vk::DispatchLoaderDynamic>>::type createValidationCacheEXTUnique( const vk::ValidationCacheCreateInfoEXT & createInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.createValidationCacheEXTUnique(createInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_NO_SMART_HANDLE*/
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result debugMarkerSetObjectNameEXT( const vk::DebugMarkerObjectNameInfoEXT* pNameInfo ) const {
		handle.debugMarkerSetObjectNameEXT(pNameInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type debugMarkerSetObjectNameEXT( const vk::DebugMarkerObjectNameInfoEXT & nameInfo ) const {
		handle.debugMarkerSetObjectNameEXT(nameInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result debugMarkerSetObjectTagEXT( const vk::DebugMarkerObjectTagInfoEXT* pTagInfo ) const {
		handle.debugMarkerSetObjectTagEXT(pTagInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type debugMarkerSetObjectTagEXT( const vk::DebugMarkerObjectTagInfoEXT & tagInfo ) const {
		handle.debugMarkerSetObjectTagEXT(tagInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyAccelerationStructureNV( vk::AccelerationStructureNV accelerationStructure, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyAccelerationStructureNV(accelerationStructure, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyAccelerationStructureNV( vk::AccelerationStructureNV accelerationStructure, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyAccelerationStructureNV(accelerationStructure, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::AccelerationStructureNV accelerationStructure, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(accelerationStructure, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::AccelerationStructureNV accelerationStructure, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(accelerationStructure, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyBuffer( vk::Buffer buffer, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyBuffer(buffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyBuffer( vk::Buffer buffer, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyBuffer(buffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Buffer buffer, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(buffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Buffer buffer, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(buffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyBufferView( vk::BufferView bufferView, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyBufferView(bufferView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyBufferView( vk::BufferView bufferView, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyBufferView(bufferView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::BufferView bufferView, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(bufferView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::BufferView bufferView, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(bufferView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyCommandPool( vk::CommandPool commandPool, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyCommandPool(commandPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyCommandPool( vk::CommandPool commandPool, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyCommandPool(commandPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::CommandPool commandPool, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(commandPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::CommandPool commandPool, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(commandPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyDescriptorPool( vk::DescriptorPool descriptorPool, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyDescriptorPool(descriptorPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyDescriptorPool( vk::DescriptorPool descriptorPool, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyDescriptorPool(descriptorPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::DescriptorPool descriptorPool, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(descriptorPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::DescriptorPool descriptorPool, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(descriptorPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyDescriptorSetLayout( vk::DescriptorSetLayout descriptorSetLayout, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyDescriptorSetLayout(descriptorSetLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyDescriptorSetLayout( vk::DescriptorSetLayout descriptorSetLayout, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyDescriptorSetLayout(descriptorSetLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::DescriptorSetLayout descriptorSetLayout, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(descriptorSetLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::DescriptorSetLayout descriptorSetLayout, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(descriptorSetLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyDescriptorUpdateTemplate( vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyDescriptorUpdateTemplate(descriptorUpdateTemplate, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyDescriptorUpdateTemplate( vk::DescriptorUpdateTemplate descriptorUpdateTemplate, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyDescriptorUpdateTemplate(descriptorUpdateTemplate, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(descriptorUpdateTemplate, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::DescriptorUpdateTemplate descriptorUpdateTemplate, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(descriptorUpdateTemplate, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyDescriptorUpdateTemplateKHR( vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyDescriptorUpdateTemplateKHR(descriptorUpdateTemplate, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyDescriptorUpdateTemplateKHR( vk::DescriptorUpdateTemplate descriptorUpdateTemplate, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyDescriptorUpdateTemplateKHR(descriptorUpdateTemplate, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyEvent( vk::Event event, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyEvent(event, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyEvent( vk::Event event, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyEvent(event, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Event event, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(event, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Event event, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(event, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyFence( vk::Fence fence, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyFence(fence, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyFence( vk::Fence fence, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyFence(fence, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Fence fence, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(fence, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Fence fence, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(fence, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyFramebuffer( vk::Framebuffer framebuffer, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyFramebuffer(framebuffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyFramebuffer( vk::Framebuffer framebuffer, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyFramebuffer(framebuffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Framebuffer framebuffer, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(framebuffer, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Framebuffer framebuffer, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(framebuffer, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyImage( vk::Image image, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyImage(image, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyImage( vk::Image image, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyImage(image, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Image image, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(image, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Image image, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(image, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyImageView( vk::ImageView imageView, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyImageView(imageView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyImageView( vk::ImageView imageView, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyImageView(imageView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::ImageView imageView, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(imageView, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::ImageView imageView, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(imageView, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyIndirectCommandsLayoutNVX( vk::IndirectCommandsLayoutNVX indirectCommandsLayout, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyIndirectCommandsLayoutNVX(indirectCommandsLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyIndirectCommandsLayoutNVX( vk::IndirectCommandsLayoutNVX indirectCommandsLayout, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyIndirectCommandsLayoutNVX(indirectCommandsLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::IndirectCommandsLayoutNVX indirectCommandsLayout, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(indirectCommandsLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::IndirectCommandsLayoutNVX indirectCommandsLayout, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(indirectCommandsLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyObjectTableNVX( vk::ObjectTableNVX objectTable, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyObjectTableNVX(objectTable, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyObjectTableNVX( vk::ObjectTableNVX objectTable, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyObjectTableNVX(objectTable, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::ObjectTableNVX objectTable, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(objectTable, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::ObjectTableNVX objectTable, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(objectTable, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyPipeline( vk::Pipeline pipeline, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyPipeline(pipeline, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyPipeline( vk::Pipeline pipeline, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyPipeline(pipeline, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Pipeline pipeline, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(pipeline, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Pipeline pipeline, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(pipeline, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyPipelineCache( vk::PipelineCache pipelineCache, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyPipelineCache(pipelineCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyPipelineCache( vk::PipelineCache pipelineCache, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyPipelineCache(pipelineCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::PipelineCache pipelineCache, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(pipelineCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::PipelineCache pipelineCache, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(pipelineCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyPipelineLayout( vk::PipelineLayout pipelineLayout, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyPipelineLayout(pipelineLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyPipelineLayout( vk::PipelineLayout pipelineLayout, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyPipelineLayout(pipelineLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::PipelineLayout pipelineLayout, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(pipelineLayout, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::PipelineLayout pipelineLayout, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(pipelineLayout, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyQueryPool( vk::QueryPool queryPool, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyQueryPool(queryPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyQueryPool( vk::QueryPool queryPool, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyQueryPool(queryPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::QueryPool queryPool, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(queryPool, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::QueryPool queryPool, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(queryPool, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyRenderPass( vk::RenderPass renderPass, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyRenderPass(renderPass, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyRenderPass( vk::RenderPass renderPass, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyRenderPass(renderPass, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::RenderPass renderPass, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(renderPass, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::RenderPass renderPass, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(renderPass, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroySampler( vk::Sampler sampler, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroySampler(sampler, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroySampler( vk::Sampler sampler, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroySampler(sampler, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Sampler sampler, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(sampler, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Sampler sampler, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(sampler, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroySamplerYcbcrConversion( vk::SamplerYcbcrConversion ycbcrConversion, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroySamplerYcbcrConversion(ycbcrConversion, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroySamplerYcbcrConversion( vk::SamplerYcbcrConversion ycbcrConversion, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroySamplerYcbcrConversion(ycbcrConversion, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::SamplerYcbcrConversion ycbcrConversion, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(ycbcrConversion, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::SamplerYcbcrConversion ycbcrConversion, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(ycbcrConversion, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroySamplerYcbcrConversionKHR( vk::SamplerYcbcrConversion ycbcrConversion, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroySamplerYcbcrConversionKHR(ycbcrConversion, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroySamplerYcbcrConversionKHR( vk::SamplerYcbcrConversion ycbcrConversion, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroySamplerYcbcrConversionKHR(ycbcrConversion, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroySemaphore( vk::Semaphore semaphore, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroySemaphore(semaphore, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroySemaphore( vk::Semaphore semaphore, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroySemaphore(semaphore, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::Semaphore semaphore, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(semaphore, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::Semaphore semaphore, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(semaphore, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyShaderModule( vk::ShaderModule shaderModule, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyShaderModule(shaderModule, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyShaderModule( vk::ShaderModule shaderModule, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyShaderModule(shaderModule, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::ShaderModule shaderModule, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(shaderModule, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::ShaderModule shaderModule, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(shaderModule, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroySwapchainKHR( vk::SwapchainKHR swapchain, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroySwapchainKHR(swapchain, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroySwapchainKHR( vk::SwapchainKHR swapchain, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroySwapchainKHR(swapchain, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::SwapchainKHR swapchain, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(swapchain, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::SwapchainKHR swapchain, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(swapchain, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroyValidationCacheEXT( vk::ValidationCacheEXT validationCache, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroyValidationCacheEXT(validationCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroyValidationCacheEXT( vk::ValidationCacheEXT validationCache, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroyValidationCacheEXT(validationCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void destroy( vk::ValidationCacheEXT validationCache, const vk::AllocationCallbacks* pAllocator ) const {
		handle.destroy(validationCache, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void destroy( vk::ValidationCacheEXT validationCache, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.destroy(validationCache, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result waitIdle( ) const {
		handle.waitIdle(dispatch);
	}
#else
 
	vk::ResultValueType<void>::type waitIdle( ) const {
		handle.waitIdle(dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result displayPowerControlEXT( vk::DisplayKHR display, const vk::DisplayPowerInfoEXT* pDisplayPowerInfo ) const {
		handle.displayPowerControlEXT(display, pDisplayPowerInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type displayPowerControlEXT( vk::DisplayKHR display, const vk::DisplayPowerInfoEXT & displayPowerInfo ) const {
		handle.displayPowerControlEXT(display, displayPowerInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result flushMappedMemoryRanges( uint32_t memoryRangeCount, const vk::MappedMemoryRange* pMemoryRanges ) const {
		handle.flushMappedMemoryRanges(memoryRangeCount, pMemoryRanges, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type flushMappedMemoryRanges( vk::ArrayProxy<const vk::MappedMemoryRange> memoryRanges ) const {
		handle.flushMappedMemoryRanges(memoryRanges, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void freeCommandBuffers( vk::CommandPool commandPool, uint32_t commandBufferCount, const vk::CommandBuffer* pCommandBuffers ) const {
		handle.freeCommandBuffers(commandPool, commandBufferCount, pCommandBuffers, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void freeCommandBuffers( vk::CommandPool commandPool, vk::ArrayProxy<const vk::CommandBuffer> commandBuffers ) const {
		handle.freeCommandBuffers(commandPool, commandBuffers, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void free( vk::CommandPool commandPool, uint32_t commandBufferCount, const vk::CommandBuffer* pCommandBuffers ) const {
		handle.free(commandPool, commandBufferCount, pCommandBuffers, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void free( vk::CommandPool commandPool, vk::ArrayProxy<const vk::CommandBuffer> commandBuffers ) const {
		handle.free(commandPool, commandBuffers, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result freeDescriptorSets( vk::DescriptorPool descriptorPool, uint32_t descriptorSetCount, const vk::DescriptorSet* pDescriptorSets ) const {
		handle.freeDescriptorSets(descriptorPool, descriptorSetCount, pDescriptorSets, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type freeDescriptorSets( vk::DescriptorPool descriptorPool, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets ) const {
		handle.freeDescriptorSets(descriptorPool, descriptorSets, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result free( vk::DescriptorPool descriptorPool, uint32_t descriptorSetCount, const vk::DescriptorSet* pDescriptorSets ) const {
		handle.free(descriptorPool, descriptorSetCount, pDescriptorSets, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type free( vk::DescriptorPool descriptorPool, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets ) const {
		handle.free(descriptorPool, descriptorSets, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void freeMemory( vk::DeviceMemory memory, const vk::AllocationCallbacks* pAllocator ) const {
		handle.freeMemory(memory, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void freeMemory( vk::DeviceMemory memory, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.freeMemory(memory, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void free( vk::DeviceMemory memory, const vk::AllocationCallbacks* pAllocator ) const {
		handle.free(memory, pAllocator, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void free( vk::DeviceMemory memory, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.free(memory, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getAccelerationStructureHandleNV( vk::AccelerationStructureNV accelerationStructure, size_t dataSize, void* pData ) const {
		handle.getAccelerationStructureHandleNV(accelerationStructure, dataSize, pData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename T>
	vk::ResultValueType<void>::type getAccelerationStructureHandleNV( vk::AccelerationStructureNV accelerationStructure, vk::ArrayProxy<T> data ) const {
		handle.getAccelerationStructureHandleNV(accelerationStructure, data, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getAccelerationStructureMemoryRequirementsNV( const vk::AccelerationStructureMemoryRequirementsInfoNV* pInfo, vk::MemoryRequirements2KHR* pMemoryRequirements ) const {
		handle.getAccelerationStructureMemoryRequirementsNV(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements2KHR getAccelerationStructureMemoryRequirementsNV( const vk::AccelerationStructureMemoryRequirementsInfoNV & info ) const {
		handle.getAccelerationStructureMemoryRequirementsNV(info, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getAccelerationStructureMemoryRequirementsNV( const vk::AccelerationStructureMemoryRequirementsInfoNV & info ) const {
		handle.getAccelerationStructureMemoryRequirementsNV(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
 
	vk::Result getAndroidHardwareBufferPropertiesANDROID( const struct vk::AHardwareBuffer* buffer, AndroidHardwareBufferPropertiesANDROID* pProperties ) const {
		handle.getAndroidHardwareBufferPropertiesANDROID(buffer, pProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<AndroidHardwareBufferPropertiesANDROID>::type getAndroidHardwareBufferPropertiesANDROID( const struct vk::AHardwareBuffer & buffer ) const {
		handle.getAndroidHardwareBufferPropertiesANDROID(buffer, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	typename vk::ResultValueType<vk::StructureChain<X, Y, Z...>>::type getAndroidHardwareBufferPropertiesANDROID( const struct vk::AHardwareBuffer & buffer ) const {
		handle.getAndroidHardwareBufferPropertiesANDROID(buffer, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

 
	vk::DeviceAddress getBufferAddressEXT( const vk::BufferDeviceAddressInfoEXT* pInfo ) const {
		handle.getBufferAddressEXT(pInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::DeviceAddress getBufferAddressEXT( const vk::BufferDeviceAddressInfoEXT & info ) const {
		handle.getBufferAddressEXT(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getBufferMemoryRequirements( vk::Buffer buffer, vk::MemoryRequirements* pMemoryRequirements ) const {
		handle.getBufferMemoryRequirements(buffer, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements getBufferMemoryRequirements( vk::Buffer buffer ) const {
		handle.getBufferMemoryRequirements(buffer, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getBufferMemoryRequirements2( const vk::BufferMemoryRequirementsInfo2* pInfo, vk::MemoryRequirements2* pMemoryRequirements ) const {
		handle.getBufferMemoryRequirements2(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements2 getBufferMemoryRequirements2( const vk::BufferMemoryRequirementsInfo2 & info ) const {
		handle.getBufferMemoryRequirements2(info, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getBufferMemoryRequirements2( const vk::BufferMemoryRequirementsInfo2 & info ) const {
		handle.getBufferMemoryRequirements2(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getBufferMemoryRequirements2KHR( const vk::BufferMemoryRequirementsInfo2* pInfo, vk::MemoryRequirements2* pMemoryRequirements ) const {
		handle.getBufferMemoryRequirements2KHR(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements2 getBufferMemoryRequirements2KHR( const vk::BufferMemoryRequirementsInfo2 & info ) const {
		handle.getBufferMemoryRequirements2KHR(info, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getBufferMemoryRequirements2KHR( const vk::BufferMemoryRequirementsInfo2 & info ) const {
		handle.getBufferMemoryRequirements2KHR(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getCalibratedTimestampsEXT( uint32_t timestampCount, const vk::CalibratedTimestampInfoEXT* pTimestampInfos, uint64_t* pTimestamps, uint64_t* pMaxDeviation ) const {
		handle.getCalibratedTimestampsEXT(timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<uint64_t>::type getCalibratedTimestampsEXT( vk::ArrayProxy<const vk::CalibratedTimestampInfoEXT> timestampInfos, vk::ArrayProxy<uint64_t> timestamps ) const {
		handle.getCalibratedTimestampsEXT(timestampInfos, timestamps, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getDescriptorSetLayoutSupport( const vk::DescriptorSetLayoutCreateInfo* pCreateInfo, vk::DescriptorSetLayoutSupport* pSupport ) const {
		handle.getDescriptorSetLayoutSupport(pCreateInfo, pSupport, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::DescriptorSetLayoutSupport getDescriptorSetLayoutSupport( const vk::DescriptorSetLayoutCreateInfo & createInfo ) const {
		handle.getDescriptorSetLayoutSupport(createInfo, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getDescriptorSetLayoutSupport( const vk::DescriptorSetLayoutCreateInfo & createInfo ) const {
		handle.getDescriptorSetLayoutSupport(createInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getDescriptorSetLayoutSupportKHR( const vk::DescriptorSetLayoutCreateInfo* pCreateInfo, vk::DescriptorSetLayoutSupport* pSupport ) const {
		handle.getDescriptorSetLayoutSupportKHR(pCreateInfo, pSupport, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::DescriptorSetLayoutSupport getDescriptorSetLayoutSupportKHR( const vk::DescriptorSetLayoutCreateInfo & createInfo ) const {
		handle.getDescriptorSetLayoutSupportKHR(createInfo, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getDescriptorSetLayoutSupportKHR( const vk::DescriptorSetLayoutCreateInfo & createInfo ) const {
		handle.getDescriptorSetLayoutSupportKHR(createInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getGroupPeerMemoryFeatures( uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, vk::PeerMemoryFeatureFlags* pPeerMemoryFeatures ) const {
		handle.getGroupPeerMemoryFeatures(heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::PeerMemoryFeatureFlags getGroupPeerMemoryFeatures( uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex ) const {
		handle.getGroupPeerMemoryFeatures(heapIndex, localDeviceIndex, remoteDeviceIndex, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getGroupPeerMemoryFeaturesKHR( uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex, vk::PeerMemoryFeatureFlags* pPeerMemoryFeatures ) const {
		handle.getGroupPeerMemoryFeaturesKHR(heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::PeerMemoryFeatureFlags getGroupPeerMemoryFeaturesKHR( uint32_t heapIndex, uint32_t localDeviceIndex, uint32_t remoteDeviceIndex ) const {
		handle.getGroupPeerMemoryFeaturesKHR(heapIndex, localDeviceIndex, remoteDeviceIndex, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getGroupPresentCapabilitiesKHR( vk::DeviceGroupPresentCapabilitiesKHR* pDeviceGroupPresentCapabilities ) const {
		handle.getGroupPresentCapabilitiesKHR(pDeviceGroupPresentCapabilities, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DeviceGroupPresentCapabilitiesKHR>::type getGroupPresentCapabilitiesKHR( ) const {
		handle.getGroupPresentCapabilitiesKHR(dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result getGroupSurfacePresentModes2EXT( const vk::PhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, vk::DeviceGroupPresentModeFlagsKHR* pModes ) const {
		handle.getGroupSurfacePresentModes2EXT(pSurfaceInfo, pModes, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type getGroupSurfacePresentModes2EXT( const vk::PhysicalDeviceSurfaceInfo2KHR & surfaceInfo ) const {
		handle.getGroupSurfacePresentModes2EXT(surfaceInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

 
	vk::Result getGroupSurfacePresentModesKHR( vk::SurfaceKHR surface, vk::DeviceGroupPresentModeFlagsKHR* pModes ) const {
		handle.getGroupSurfacePresentModesKHR(surface, pModes, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::DeviceGroupPresentModeFlagsKHR>::type getGroupSurfacePresentModesKHR( vk::SurfaceKHR surface ) const {
		handle.getGroupSurfacePresentModesKHR(surface, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getMemoryCommitment( vk::DeviceMemory memory, vk::DeviceSize* pCommittedMemoryInBytes ) const {
		handle.getMemoryCommitment(memory, pCommittedMemoryInBytes, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::DeviceSize getMemoryCommitment( vk::DeviceMemory memory ) const {
		handle.getMemoryCommitment(memory, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	PFN_vkVoidFunction getProcAddr( const char* pName ) const {
		handle.getProcAddr(pName, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	PFN_vkVoidFunction getProcAddr( const std::string & name ) const {
		handle.getProcAddr(name, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex, vk::Queue* pQueue ) const {
		handle.getQueue(queueFamilyIndex, queueIndex, pQueue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Queue getQueue( uint32_t queueFamilyIndex, uint32_t queueIndex ) const {
		handle.getQueue(queueFamilyIndex, queueIndex, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getQueue2( const vk::DeviceQueueInfo2* pQueueInfo, vk::Queue* pQueue ) const {
		handle.getQueue2(pQueueInfo, pQueue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Queue getQueue2( const vk::DeviceQueueInfo2 & queueInfo ) const {
		handle.getQueue2(queueInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getEventStatus( vk::Event event ) const {
		handle.getEventStatus(event, dispatch);
	}

 
	vk::Result getFenceFdKHR( const vk::FenceGetFdInfoKHR* pGetFdInfo, int* pFd ) const {
		handle.getFenceFdKHR(pGetFdInfo, pFd, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<int>::type getFenceFdKHR( const vk::FenceGetFdInfoKHR & getFdInfo ) const {
		handle.getFenceFdKHR(getFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getFenceStatus( vk::Fence fence ) const {
		handle.getFenceStatus(fence, dispatch);
	}

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result getFenceWin32HandleKHR( const vk::FenceGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle ) const {
		handle.getFenceWin32HandleKHR(pGetWin32HandleInfo, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<HANDLE>::type getFenceWin32HandleKHR( const vk::FenceGetWin32HandleInfoKHR & getWin32HandleInfo ) const {
		handle.getFenceWin32HandleKHR(getWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

 
	vk::Result getImageDrmFormatModifierPropertiesEXT( vk::Image image, vk::ImageDrmFormatModifierPropertiesEXT* pProperties ) const {
		handle.getImageDrmFormatModifierPropertiesEXT(image, pProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::ImageDrmFormatModifierPropertiesEXT>::type getImageDrmFormatModifierPropertiesEXT( vk::Image image ) const {
		handle.getImageDrmFormatModifierPropertiesEXT(image, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageMemoryRequirements( vk::Image image, vk::MemoryRequirements* pMemoryRequirements ) const {
		handle.getImageMemoryRequirements(image, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements getImageMemoryRequirements( vk::Image image ) const {
		handle.getImageMemoryRequirements(image, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageMemoryRequirements2( const vk::ImageMemoryRequirementsInfo2* pInfo, vk::MemoryRequirements2* pMemoryRequirements ) const {
		handle.getImageMemoryRequirements2(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements2 getImageMemoryRequirements2( const vk::ImageMemoryRequirementsInfo2 & info ) const {
		handle.getImageMemoryRequirements2(info, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getImageMemoryRequirements2( const vk::ImageMemoryRequirementsInfo2 & info ) const {
		handle.getImageMemoryRequirements2(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageMemoryRequirements2KHR( const vk::ImageMemoryRequirementsInfo2* pInfo, vk::MemoryRequirements2* pMemoryRequirements ) const {
		handle.getImageMemoryRequirements2KHR(pInfo, pMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::MemoryRequirements2 getImageMemoryRequirements2KHR( const vk::ImageMemoryRequirementsInfo2 & info ) const {
		handle.getImageMemoryRequirements2KHR(info, dispatch);
	}
	template<typename X, typename Y, typename ...Z>
	vk::StructureChain<X, Y, Z...> getImageMemoryRequirements2KHR( const vk::ImageMemoryRequirementsInfo2 & info ) const {
		handle.getImageMemoryRequirements2KHR(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageSparseMemoryRequirements( vk::Image image, uint32_t* pSparseMemoryRequirementCount, vk::SparseImageMemoryRequirements* pSparseMemoryRequirements ) const {
		handle.getImageSparseMemoryRequirements(image, pSparseMemoryRequirementCount, pSparseMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::SparseImageMemoryRequirements>>
	std::vector<vk::SparseImageMemoryRequirements,Allocator> getImageSparseMemoryRequirements( vk::Image image ) const {
		handle.getImageSparseMemoryRequirements(image, dispatch);
	}
	template<typename Allocator = std::allocator<vk::SparseImageMemoryRequirements>>
	std::vector<vk::SparseImageMemoryRequirements,Allocator> getImageSparseMemoryRequirements( vk::Image image, Allocator const& vectorAllocator ) const {
		handle.getImageSparseMemoryRequirements(image, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageSparseMemoryRequirements2( const vk::ImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, vk::SparseImageMemoryRequirements2* pSparseMemoryRequirements ) const {
		handle.getImageSparseMemoryRequirements2(pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::SparseImageMemoryRequirements2>>
	std::vector<vk::SparseImageMemoryRequirements2,Allocator> getImageSparseMemoryRequirements2( const vk::ImageSparseMemoryRequirementsInfo2 & info ) const {
		handle.getImageSparseMemoryRequirements2(info, dispatch);
	}
	template<typename Allocator = std::allocator<vk::SparseImageMemoryRequirements2>>
	std::vector<vk::SparseImageMemoryRequirements2,Allocator> getImageSparseMemoryRequirements2( const vk::ImageSparseMemoryRequirementsInfo2 & info, Allocator const& vectorAllocator ) const {
		handle.getImageSparseMemoryRequirements2(info, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageSparseMemoryRequirements2KHR( const vk::ImageSparseMemoryRequirementsInfo2* pInfo, uint32_t* pSparseMemoryRequirementCount, vk::SparseImageMemoryRequirements2* pSparseMemoryRequirements ) const {
		handle.getImageSparseMemoryRequirements2KHR(pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::SparseImageMemoryRequirements2>>
	std::vector<vk::SparseImageMemoryRequirements2,Allocator> getImageSparseMemoryRequirements2KHR( const vk::ImageSparseMemoryRequirementsInfo2 & info ) const {
		handle.getImageSparseMemoryRequirements2KHR(info, dispatch);
	}
	template<typename Allocator = std::allocator<vk::SparseImageMemoryRequirements2>>
	std::vector<vk::SparseImageMemoryRequirements2,Allocator> getImageSparseMemoryRequirements2KHR( const vk::ImageSparseMemoryRequirementsInfo2 & info, Allocator const& vectorAllocator ) const {
		handle.getImageSparseMemoryRequirements2KHR(info, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getImageSubresourceLayout( vk::Image image, const vk::ImageSubresource* pSubresource, vk::SubresourceLayout* pLayout ) const {
		handle.getImageSubresourceLayout(image, pSubresource, pLayout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::SubresourceLayout getImageSubresourceLayout( vk::Image image, const vk::ImageSubresource & subresource ) const {
		handle.getImageSubresourceLayout(image, subresource, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	uint32_t getImageViewHandleNVX( const vk::ImageViewHandleInfoNVX* pInfo ) const {
		handle.getImageViewHandleNVX(pInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	uint32_t getImageViewHandleNVX( const vk::ImageViewHandleInfoNVX & info ) const {
		handle.getImageViewHandleNVX(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_ANDROID_KHR
 
	vk::Result getMemoryAndroidHardwareBufferANDROID( const vk::MemoryGetAndroidHardwareBufferInfoANDROID* pInfo, struct vk::AHardwareBuffer** pBuffer ) const {
		handle.getMemoryAndroidHardwareBufferANDROID(pInfo, pBuffer, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<struct vk::AHardwareBuffer*>::type getMemoryAndroidHardwareBufferANDROID( const vk::MemoryGetAndroidHardwareBufferInfoANDROID & info ) const {
		handle.getMemoryAndroidHardwareBufferANDROID(info, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_ANDROID_KHR*/

 
	vk::Result getMemoryFdKHR( const vk::MemoryGetFdInfoKHR* pGetFdInfo, int* pFd ) const {
		handle.getMemoryFdKHR(pGetFdInfo, pFd, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<int>::type getMemoryFdKHR( const vk::MemoryGetFdInfoKHR & getFdInfo ) const {
		handle.getMemoryFdKHR(getFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getMemoryFdPropertiesKHR( vk::ExternalMemoryHandleTypeFlagBits handleType, int fd, vk::MemoryFdPropertiesKHR* pMemoryFdProperties ) const {
		handle.getMemoryFdPropertiesKHR(handleType, fd, pMemoryFdProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::MemoryFdPropertiesKHR>::type getMemoryFdPropertiesKHR( vk::ExternalMemoryHandleTypeFlagBits handleType, int fd ) const {
		handle.getMemoryFdPropertiesKHR(handleType, fd, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getMemoryHostPointerPropertiesEXT( vk::ExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer, vk::MemoryHostPointerPropertiesEXT* pMemoryHostPointerProperties ) const {
		handle.getMemoryHostPointerPropertiesEXT(handleType, pHostPointer, pMemoryHostPointerProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::MemoryHostPointerPropertiesEXT>::type getMemoryHostPointerPropertiesEXT( vk::ExternalMemoryHandleTypeFlagBits handleType, const void* pHostPointer ) const {
		handle.getMemoryHostPointerPropertiesEXT(handleType, pHostPointer, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result getMemoryWin32HandleKHR( const vk::MemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle ) const {
		handle.getMemoryWin32HandleKHR(pGetWin32HandleInfo, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<HANDLE>::type getMemoryWin32HandleKHR( const vk::MemoryGetWin32HandleInfoKHR & getWin32HandleInfo ) const {
		handle.getMemoryWin32HandleKHR(getWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result getMemoryWin32HandleNV( vk::DeviceMemory memory, vk::ExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle ) const {
		handle.getMemoryWin32HandleNV(memory, handleType, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<HANDLE>::type getMemoryWin32HandleNV( vk::DeviceMemory memory, vk::ExternalMemoryHandleTypeFlagsNV handleType ) const {
		handle.getMemoryWin32HandleNV(memory, handleType, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result getMemoryWin32HandlePropertiesKHR( vk::ExternalMemoryHandleTypeFlagBits handleType, HANDLE handle, vk::MemoryWin32HandlePropertiesKHR* pMemoryWin32HandleProperties ) const {
		handle.getMemoryWin32HandlePropertiesKHR(handleType, handle, pMemoryWin32HandleProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::MemoryWin32HandlePropertiesKHR>::type getMemoryWin32HandlePropertiesKHR( vk::ExternalMemoryHandleTypeFlagBits handleType, HANDLE handle ) const {
		handle.getMemoryWin32HandlePropertiesKHR(handleType, handle, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

 
	vk::Result getPastPresentationTimingGOOGLE( vk::SwapchainKHR swapchain, uint32_t* pPresentationTimingCount, vk::PastPresentationTimingGOOGLE* pPresentationTimings ) const {
		handle.getPastPresentationTimingGOOGLE(swapchain, pPresentationTimingCount, pPresentationTimings, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::PastPresentationTimingGOOGLE>>
	typename vk::ResultValueType<std::vector<vk::PastPresentationTimingGOOGLE,Allocator>>::type getPastPresentationTimingGOOGLE( vk::SwapchainKHR swapchain ) const {
		handle.getPastPresentationTimingGOOGLE(swapchain, dispatch);
	}
	template<typename Allocator = std::allocator<vk::PastPresentationTimingGOOGLE>>
	typename vk::ResultValueType<std::vector<vk::PastPresentationTimingGOOGLE,Allocator>>::type getPastPresentationTimingGOOGLE( vk::SwapchainKHR swapchain, Allocator const& vectorAllocator ) const {
		handle.getPastPresentationTimingGOOGLE(swapchain, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getPerformanceParameterINTEL( vk::PerformanceParameterTypeINTEL parameter, vk::PerformanceValueINTEL* pValue ) const {
		handle.getPerformanceParameterINTEL(parameter, pValue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::PerformanceValueINTEL>::type getPerformanceParameterINTEL( vk::PerformanceParameterTypeINTEL parameter ) const {
		handle.getPerformanceParameterINTEL(parameter, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getPipelineCacheData( vk::PipelineCache pipelineCache, size_t* pDataSize, void* pData ) const {
		handle.getPipelineCacheData(pipelineCache, pDataSize, pData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<uint8_t>>
	typename vk::ResultValueType<std::vector<uint8_t,Allocator>>::type getPipelineCacheData( vk::PipelineCache pipelineCache ) const {
		handle.getPipelineCacheData(pipelineCache, dispatch);
	}
	template<typename Allocator = std::allocator<uint8_t>>
	typename vk::ResultValueType<std::vector<uint8_t,Allocator>>::type getPipelineCacheData( vk::PipelineCache pipelineCache, Allocator const& vectorAllocator ) const {
		handle.getPipelineCacheData(pipelineCache, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getQueryPoolResults( vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, size_t dataSize, void* pData, vk::DeviceSize stride, vk::QueryResultFlags flags ) const {
		handle.getQueryPoolResults(queryPool, firstQuery, queryCount, dataSize, pData, stride, flags, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename T>
	vk::Result getQueryPoolResults( vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount, vk::ArrayProxy<T> data, vk::DeviceSize stride, vk::QueryResultFlags flags ) const {
		handle.getQueryPoolResults(queryPool, firstQuery, queryCount, data, stride, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getRayTracingShaderGroupHandlesNV( vk::Pipeline pipeline, uint32_t firstGroup, uint32_t groupCount, size_t dataSize, void* pData ) const {
		handle.getRayTracingShaderGroupHandlesNV(pipeline, firstGroup, groupCount, dataSize, pData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename T>
	vk::ResultValueType<void>::type getRayTracingShaderGroupHandlesNV( vk::Pipeline pipeline, uint32_t firstGroup, uint32_t groupCount, vk::ArrayProxy<T> data ) const {
		handle.getRayTracingShaderGroupHandlesNV(pipeline, firstGroup, groupCount, data, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getRefreshCycleDurationGOOGLE( vk::SwapchainKHR swapchain, vk::RefreshCycleDurationGOOGLE* pDisplayTimingProperties ) const {
		handle.getRefreshCycleDurationGOOGLE(swapchain, pDisplayTimingProperties, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::RefreshCycleDurationGOOGLE>::type getRefreshCycleDurationGOOGLE( vk::SwapchainKHR swapchain ) const {
		handle.getRefreshCycleDurationGOOGLE(swapchain, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void getRenderAreaGranularity( vk::RenderPass renderPass, vk::Extent2D* pGranularity ) const {
		handle.getRenderAreaGranularity(renderPass, pGranularity, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Extent2D getRenderAreaGranularity( vk::RenderPass renderPass ) const {
		handle.getRenderAreaGranularity(renderPass, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getSemaphoreFdKHR( const vk::SemaphoreGetFdInfoKHR* pGetFdInfo, int* pFd ) const {
		handle.getSemaphoreFdKHR(pGetFdInfo, pFd, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<int>::type getSemaphoreFdKHR( const vk::SemaphoreGetFdInfoKHR & getFdInfo ) const {
		handle.getSemaphoreFdKHR(getFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result getSemaphoreWin32HandleKHR( const vk::SemaphoreGetWin32HandleInfoKHR* pGetWin32HandleInfo, HANDLE* pHandle ) const {
		handle.getSemaphoreWin32HandleKHR(pGetWin32HandleInfo, pHandle, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<HANDLE>::type getSemaphoreWin32HandleKHR( const vk::SemaphoreGetWin32HandleInfoKHR & getWin32HandleInfo ) const {
		handle.getSemaphoreWin32HandleKHR(getWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

 
	vk::Result getShaderInfoAMD( vk::Pipeline pipeline, vk::ShaderStageFlagBits shaderStage, vk::ShaderInfoTypeAMD infoType, size_t* pInfoSize, void* pInfo ) const {
		handle.getShaderInfoAMD(pipeline, shaderStage, infoType, pInfoSize, pInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<uint8_t>>
	typename vk::ResultValueType<std::vector<uint8_t,Allocator>>::type getShaderInfoAMD( vk::Pipeline pipeline, vk::ShaderStageFlagBits shaderStage, vk::ShaderInfoTypeAMD infoType ) const {
		handle.getShaderInfoAMD(pipeline, shaderStage, infoType, dispatch);
	}
	template<typename Allocator = std::allocator<uint8_t>>
	typename vk::ResultValueType<std::vector<uint8_t,Allocator>>::type getShaderInfoAMD( vk::Pipeline pipeline, vk::ShaderStageFlagBits shaderStage, vk::ShaderInfoTypeAMD infoType, Allocator const& vectorAllocator ) const {
		handle.getShaderInfoAMD(pipeline, shaderStage, infoType, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getSwapchainCounterEXT( vk::SwapchainKHR swapchain, vk::SurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue ) const {
		handle.getSwapchainCounterEXT(swapchain, counter, pCounterValue, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<uint64_t>::type getSwapchainCounterEXT( vk::SwapchainKHR swapchain, vk::SurfaceCounterFlagBitsEXT counter ) const {
		handle.getSwapchainCounterEXT(swapchain, counter, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getSwapchainImagesKHR( vk::SwapchainKHR swapchain, uint32_t* pSwapchainImageCount, vk::Image* pSwapchainImages ) const {
		handle.getSwapchainImagesKHR(swapchain, pSwapchainImageCount, pSwapchainImages, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<vk::Image>>
	typename vk::ResultValueType<std::vector<vk::Image,Allocator>>::type getSwapchainImagesKHR( vk::SwapchainKHR swapchain ) const {
		handle.getSwapchainImagesKHR(swapchain, dispatch);
	}
	template<typename Allocator = std::allocator<vk::Image>>
	typename vk::ResultValueType<std::vector<vk::Image,Allocator>>::type getSwapchainImagesKHR( vk::SwapchainKHR swapchain, Allocator const& vectorAllocator ) const {
		handle.getSwapchainImagesKHR(swapchain, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result getSwapchainStatusKHR( vk::SwapchainKHR swapchain ) const {
		handle.getSwapchainStatusKHR(swapchain, dispatch);
	}

 
	vk::Result getValidationCacheDataEXT( vk::ValidationCacheEXT validationCache, size_t* pDataSize, void* pData ) const {
		handle.getValidationCacheDataEXT(validationCache, pDataSize, pData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
	template<typename Allocator = std::allocator<uint8_t>>
	typename vk::ResultValueType<std::vector<uint8_t,Allocator>>::type getValidationCacheDataEXT( vk::ValidationCacheEXT validationCache ) const {
		handle.getValidationCacheDataEXT(validationCache, dispatch);
	}
	template<typename Allocator = std::allocator<uint8_t>>
	typename vk::ResultValueType<std::vector<uint8_t,Allocator>>::type getValidationCacheDataEXT( vk::ValidationCacheEXT validationCache, Allocator const& vectorAllocator ) const {
		handle.getValidationCacheDataEXT(validationCache, vectorAllocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result importFenceFdKHR( const vk::ImportFenceFdInfoKHR* pImportFenceFdInfo ) const {
		handle.importFenceFdKHR(pImportFenceFdInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type importFenceFdKHR( const vk::ImportFenceFdInfoKHR & importFenceFdInfo ) const {
		handle.importFenceFdKHR(importFenceFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result importFenceWin32HandleKHR( const vk::ImportFenceWin32HandleInfoKHR* pImportFenceWin32HandleInfo ) const {
		handle.importFenceWin32HandleKHR(pImportFenceWin32HandleInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type importFenceWin32HandleKHR( const vk::ImportFenceWin32HandleInfoKHR & importFenceWin32HandleInfo ) const {
		handle.importFenceWin32HandleKHR(importFenceWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

 
	vk::Result importSemaphoreFdKHR( const vk::ImportSemaphoreFdInfoKHR* pImportSemaphoreFdInfo ) const {
		handle.importSemaphoreFdKHR(pImportSemaphoreFdInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type importSemaphoreFdKHR( const vk::ImportSemaphoreFdInfoKHR & importSemaphoreFdInfo ) const {
		handle.importSemaphoreFdKHR(importSemaphoreFdInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
 
	vk::Result importSemaphoreWin32HandleKHR( const vk::ImportSemaphoreWin32HandleInfoKHR* pImportSemaphoreWin32HandleInfo ) const {
		handle.importSemaphoreWin32HandleKHR(pImportSemaphoreWin32HandleInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type importSemaphoreWin32HandleKHR( const vk::ImportSemaphoreWin32HandleInfoKHR & importSemaphoreWin32HandleInfo ) const {
		handle.importSemaphoreWin32HandleKHR(importSemaphoreWin32HandleInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

 
	vk::Result initializePerformanceApiINTEL( const vk::InitializePerformanceApiInfoINTEL* pInitializeInfo ) const {
		handle.initializePerformanceApiINTEL(pInitializeInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type initializePerformanceApiINTEL( const vk::InitializePerformanceApiInfoINTEL & initializeInfo ) const {
		handle.initializePerformanceApiINTEL(initializeInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result invalidateMappedMemoryRanges( uint32_t memoryRangeCount, const vk::MappedMemoryRange* pMemoryRanges ) const {
		handle.invalidateMappedMemoryRanges(memoryRangeCount, pMemoryRanges, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type invalidateMappedMemoryRanges( vk::ArrayProxy<const vk::MappedMemoryRange> memoryRanges ) const {
		handle.invalidateMappedMemoryRanges(memoryRanges, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result mapMemory( vk::DeviceMemory memory, vk::DeviceSize offset, vk::DeviceSize size, vk::MemoryMapFlags flags, void** ppData ) const {
		handle.mapMemory(memory, offset, size, flags, ppData, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void*>::type mapMemory( vk::DeviceMemory memory, vk::DeviceSize offset, vk::DeviceSize size, vk::MemoryMapFlags flags = vk::MemoryMapFlags() ) const {
		handle.mapMemory(memory, offset, size, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result mergePipelineCaches( vk::PipelineCache dstCache, uint32_t srcCacheCount, const vk::PipelineCache* pSrcCaches ) const {
		handle.mergePipelineCaches(dstCache, srcCacheCount, pSrcCaches, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type mergePipelineCaches( vk::PipelineCache dstCache, vk::ArrayProxy<const vk::PipelineCache> srcCaches ) const {
		handle.mergePipelineCaches(dstCache, srcCaches, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result mergeValidationCachesEXT( vk::ValidationCacheEXT dstCache, uint32_t srcCacheCount, const vk::ValidationCacheEXT* pSrcCaches ) const {
		handle.mergeValidationCachesEXT(dstCache, srcCacheCount, pSrcCaches, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type mergeValidationCachesEXT( vk::ValidationCacheEXT dstCache, vk::ArrayProxy<const vk::ValidationCacheEXT> srcCaches ) const {
		handle.mergeValidationCachesEXT(dstCache, srcCaches, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result registerEventEXT( const vk::DeviceEventInfoEXT* pDeviceEventInfo, const vk::AllocationCallbacks* pAllocator, vk::Fence* pFence ) const {
		handle.registerEventEXT(pDeviceEventInfo, pAllocator, pFence, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Fence>::type registerEventEXT( const vk::DeviceEventInfoEXT & deviceEventInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.registerEventEXT(deviceEventInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result registerDisplayEventEXT( vk::DisplayKHR display, const vk::DisplayEventInfoEXT* pDisplayEventInfo, const vk::AllocationCallbacks* pAllocator, vk::Fence* pFence ) const {
		handle.registerDisplayEventEXT(display, pDisplayEventInfo, pAllocator, pFence, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<vk::Fence>::type registerDisplayEventEXT( vk::DisplayKHR display, const vk::DisplayEventInfoEXT & displayEventInfo, vk::Optional<const vk::AllocationCallbacks> allocator = nullptr ) const {
		handle.registerDisplayEventEXT(display, displayEventInfo, allocator, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result registerObjectsNVX( vk::ObjectTableNVX objectTable, uint32_t objectCount, const vk::ObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices ) const {
		handle.registerObjectsNVX(objectTable, objectCount, ppObjectTableEntries, pObjectIndices, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type registerObjectsNVX( vk::ObjectTableNVX objectTable, vk::ArrayProxy<const vk::ObjectTableEntryNVX* const> pObjectTableEntries, vk::ArrayProxy<const uint32_t> objectIndices ) const {
		handle.registerObjectsNVX(objectTable, pObjectTableEntries, objectIndices, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VK_USE_PLATFORM_WIN32_KHR
#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result releaseFullScreenExclusiveModeEXT( vk::SwapchainKHR swapchain ) const {
		handle.releaseFullScreenExclusiveModeEXT(swapchain, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type releaseFullScreenExclusiveModeEXT( vk::SwapchainKHR swapchain ) const {
		handle.releaseFullScreenExclusiveModeEXT(swapchain, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/
#endif /*VK_USE_PLATFORM_WIN32_KHR*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result releasePerformanceConfigurationINTEL( vk::PerformanceConfigurationINTEL configuration ) const {
		handle.releasePerformanceConfigurationINTEL(configuration, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type releasePerformanceConfigurationINTEL( vk::PerformanceConfigurationINTEL configuration ) const {
		handle.releasePerformanceConfigurationINTEL(configuration, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result resetCommandPool( vk::CommandPool commandPool, vk::CommandPoolResetFlags flags ) const {
		handle.resetCommandPool(commandPool, flags, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type resetCommandPool( vk::CommandPool commandPool, vk::CommandPoolResetFlags flags ) const {
		handle.resetCommandPool(commandPool, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result resetDescriptorPool( vk::DescriptorPool descriptorPool, vk::DescriptorPoolResetFlags flags = vk::DescriptorPoolResetFlags() ) const {
		handle.resetDescriptorPool(descriptorPool, flags, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type resetDescriptorPool( vk::DescriptorPool descriptorPool, vk::DescriptorPoolResetFlags flags = vk::DescriptorPoolResetFlags() ) const {
		handle.resetDescriptorPool(descriptorPool, flags, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result resetEvent( vk::Event event ) const {
		handle.resetEvent(event, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type resetEvent( vk::Event event ) const {
		handle.resetEvent(event, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result resetFences( uint32_t fenceCount, const vk::Fence* pFences ) const {
		handle.resetFences(fenceCount, pFences, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type resetFences( vk::ArrayProxy<const vk::Fence> fences ) const {
		handle.resetFences(fences, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void resetQueryPoolEXT( vk::QueryPool queryPool, uint32_t firstQuery, uint32_t queryCount ) const {
		handle.resetQueryPoolEXT(queryPool, firstQuery, queryCount, dispatch);
	}

 
	vk::Result setDebugUtilsObjectNameEXT( const vk::DebugUtilsObjectNameInfoEXT* pNameInfo ) const {
		handle.setDebugUtilsObjectNameEXT(pNameInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type setDebugUtilsObjectNameEXT( const vk::DebugUtilsObjectNameInfoEXT & nameInfo ) const {
		handle.setDebugUtilsObjectNameEXT(nameInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result setDebugUtilsObjectTagEXT( const vk::DebugUtilsObjectTagInfoEXT* pTagInfo ) const {
		handle.setDebugUtilsObjectTagEXT(pTagInfo, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type setDebugUtilsObjectTagEXT( const vk::DebugUtilsObjectTagInfoEXT & tagInfo ) const {
		handle.setDebugUtilsObjectTagEXT(tagInfo, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

#ifdef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result setEvent( vk::Event event ) const {
		handle.setEvent(event, dispatch);
	}
#else
 
	vk::ResultValueType<void>::type setEvent( vk::Event event ) const {
		handle.setEvent(event, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void setHdrMetadataEXT( uint32_t swapchainCount, const vk::SwapchainKHR* pSwapchains, const vk::HdrMetadataEXT* pMetadata ) const {
		handle.setHdrMetadataEXT(swapchainCount, pSwapchains, pMetadata, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void setHdrMetadataEXT( vk::ArrayProxy<const vk::SwapchainKHR> swapchains, vk::ArrayProxy<const vk::HdrMetadataEXT> metadata ) const {
		handle.setHdrMetadataEXT(swapchains, metadata, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void setLocalDimmingAMD( vk::SwapchainKHR swapChain, vk::Bool32 localDimmingEnable ) const {
		handle.setLocalDimmingAMD(swapChain, localDimmingEnable, dispatch);
	}

 
	void trimCommandPool( vk::CommandPool commandPool, vk::CommandPoolTrimFlags flags = vk::CommandPoolTrimFlags() ) const {
		handle.trimCommandPool(commandPool, flags, dispatch);
	}

 
	void trimCommandPoolKHR( vk::CommandPool commandPool, vk::CommandPoolTrimFlags flags = vk::CommandPoolTrimFlags() ) const {
		handle.trimCommandPoolKHR(commandPool, flags, dispatch);
	}

 
	void uninitializePerformanceApiINTEL( ) const {
		handle.uninitializePerformanceApiINTEL(dispatch);
	}

 
	void unmapMemory( vk::DeviceMemory memory ) const {
		handle.unmapMemory(memory, dispatch);
	}

 
	vk::Result unregisterObjectsNVX( vk::ObjectTableNVX objectTable, uint32_t objectCount, const vk::ObjectEntryTypeNVX* pObjectEntryTypes, const uint32_t* pObjectIndices ) const {
		handle.unregisterObjectsNVX(objectTable, objectCount, pObjectEntryTypes, pObjectIndices, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::ResultValueType<void>::type unregisterObjectsNVX( vk::ObjectTableNVX objectTable, vk::ArrayProxy<const vk::ObjectEntryTypeNVX> objectEntryTypes, vk::ArrayProxy<const uint32_t> objectIndices ) const {
		handle.unregisterObjectsNVX(objectTable, objectEntryTypes, objectIndices, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	void updateDescriptorSetWithTemplate( vk::DescriptorSet descriptorSet, vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData ) const {
		handle.updateDescriptorSetWithTemplate(descriptorSet, descriptorUpdateTemplate, pData, dispatch);
	}

 
	void updateDescriptorSetWithTemplateKHR( vk::DescriptorSet descriptorSet, vk::DescriptorUpdateTemplate descriptorUpdateTemplate, const void* pData ) const {
		handle.updateDescriptorSetWithTemplateKHR(descriptorSet, descriptorUpdateTemplate, pData, dispatch);
	}

 
	void updateDescriptorSets( uint32_t descriptorWriteCount, const vk::WriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount, const vk::CopyDescriptorSet* pDescriptorCopies ) const {
		handle.updateDescriptorSets(descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	void updateDescriptorSets( vk::ArrayProxy<const vk::WriteDescriptorSet> descriptorWrites, vk::ArrayProxy<const vk::CopyDescriptorSet> descriptorCopies ) const {
		handle.updateDescriptorSets(descriptorWrites, descriptorCopies, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/

 
	vk::Result waitForFences( uint32_t fenceCount, const vk::Fence* pFences, vk::Bool32 waitAll, uint64_t timeout ) const {
		handle.waitForFences(fenceCount, pFences, waitAll, timeout, dispatch);
	}
#ifndef VULKAN_HPP_DISABLE_ENHANCED_MODE
 
	vk::Result waitForFences( vk::ArrayProxy<const vk::Fence> fences, vk::Bool32 waitAll, uint64_t timeout ) const {
		handle.waitForFences(fences, waitAll, timeout, dispatch);
	}
#endif /*VULKAN_HPP_DISABLE_ENHANCED_MODE*/


private:

	vk::Device handle;
	vk::DispatchLoaderDynamic dispatch;
};

#endif