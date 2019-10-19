#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Vulkan.hpp"

struct Texture {
public:
	Texture();
	Texture(vk::Device, vk::DispatchLoaderDynamic);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&&);
	Texture& operator=(Texture&&);

private:
	vk::Device device;
	vk::DispatchLoaderDynamic loader;

	void cleanup();

public:
	uint32_t width, height;

	vk::DeviceMemory memory;
	vk::Image image;
	vk::ImageView view;
	uint32_t mipLevels;

};

#endif