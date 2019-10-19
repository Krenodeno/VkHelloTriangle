#ifndef VERTEX_H
#define VERTEX_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "Vulkan.hpp"

#include <array>
#include <functional>
#include <algorithm>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static vk::VertexInputBindingDescription getBindingDescription() {
		vk::VertexInputBindingDescription bindingDesc;
		bindingDesc.binding = 0;
		bindingDesc.stride = sizeof(Vertex);
		bindingDesc.inputRate = vk::VertexInputRate::eVertex;

		return bindingDesc;
	}

	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<vk::VertexInputAttributeDescription, 3> attributeDesc;
		attributeDesc[0].binding = 0;
		attributeDesc[0].format = vk::Format::eR32G32B32Sfloat;	// stand for vec3 of 32bits floats
		attributeDesc[0].location = 0;
		attributeDesc[0].offset = offsetof(Vertex, pos);

		attributeDesc[1].binding = 0;
		attributeDesc[1].location = 1;
		attributeDesc[1].format = vk::Format::eR32G32B32Sfloat;	// stand for vec3 of 32bits floats
		attributeDesc[1].offset = offsetof(Vertex, color);

		attributeDesc[2].binding = 0;
		attributeDesc[2].location = 2;
		attributeDesc[2].format = vk::Format::eR32G32Sfloat;	// stand for vec2 of 32bits floats
		attributeDesc[2].offset = offsetof(Vertex, texCoord);

		return attributeDesc;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

#endif	// VERTEX_H
