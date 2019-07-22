#include "Texture.hpp"

Texture::Texture() {
}

Texture::Texture(vk::Device dev, vk::DispatchLoaderDynamic deviceLoader) : device(dev), loader(deviceLoader) {

}

Texture::~Texture() {
	cleanup();
}

void Texture::cleanup() {
	if (view)
		device.destroyImageView(view, nullptr, loader);
	if (image)
		device.destroyImage(image, nullptr, loader);
	if (memory)
		device.freeMemory(memory, nullptr, loader);
}

Texture::Texture(Texture&& rhs) {
	cleanup();

	device = rhs.device;
	loader = rhs.loader;
	width = rhs.width;
	height = rhs.height;
	image = rhs.image;
	view = rhs.view;
	memory = rhs.memory;
	mipLevels = rhs.mipLevels;

	rhs.image = nullptr;
	rhs.view = nullptr;
	rhs.memory = nullptr;

}

Texture& Texture::operator=(Texture&& rhs) {
	cleanup();

	device = rhs.device;
	loader = rhs.loader;
	width = rhs.width;
	height = rhs.height;
	image = rhs.image;
	view = rhs.view;
	memory = rhs.memory;
	mipLevels = rhs.mipLevels;

	rhs.image = nullptr;
	rhs.view = nullptr;
	rhs.memory = nullptr;

	return *this;
}
