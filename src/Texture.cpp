#include "Texture.hpp"

Texture::Texture() {
}

Texture::Texture(vk::Device dev, vk::DispatchLoaderDynamic deviceLoader) : device(dev), loader(deviceLoader) {

}

Texture::~Texture() {
	cleanup();
}

void Texture::cleanup() {
	if (view) {
		device.destroyImageView(view, nullptr, loader);
		view = nullptr;
	}
	if (image) {
		device.destroyImage(image, nullptr, loader);
		image = nullptr;
	}
	if (memory) {
		device.freeMemory(memory, nullptr, loader);
		memory = nullptr;
	}
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
