#ifndef MESH_HPP
#define MESH_HPP

#include "Vulkan.hpp"

#include "Vertex.hpp"

class Mesh {
public:
	Mesh();
	Mesh(vk::Device, vk::DispatchLoaderDynamic);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&&);
	Mesh& operator=(Mesh&&);


	uint64_t vertexCount;
	uint64_t indexCount;

private:

	vk::Device device;
	vk::DispatchLoaderDynamic deviceLoader;

	vk::Buffer vertexbuffer;
	vk::Buffer indexBuffer;
	vk::DeviceMemory memory;

};

struct MeshData {
	std::Vector<Vertex> data;
};

#endif