#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "Vulkan.hpp"

struct Buffer {
	vk::Buffer buffer;
	vk::DeviceMemory memory;
	vk::BufferUsageFlags usage;
	uint64_t size;
	uint64_t offset;

	void cleanup(vk::Device device, vk::DispatchLoaderDynamic d) {
		if (buffer) {
			device.destroy(buffer, nullptr, d);
			buffer = nullptr;
		}
		if (memory) {
			device.free(memory, nullptr, d);
			memory = nullptr;
		}
	}
};

#endif
