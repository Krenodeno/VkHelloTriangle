#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vulkan/vulkan.h>

#include <array>
#include <functional>
#include <algorithm>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 normal;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};
		size_t attribute = 0;
		attributeDescriptions[attribute].binding = 0;
		attributeDescriptions[attribute].location = 0;
		attributeDescriptions[attribute].format = VK_FORMAT_R32G32B32_SFLOAT;	// stand for vec3 of 32bits floats
		attributeDescriptions[attribute].offset = offsetof(Vertex, pos);

		attribute++;
		attributeDescriptions[attribute].binding = 0;
		attributeDescriptions[attribute].location = 1;
		attributeDescriptions[attribute].format = VK_FORMAT_R32G32B32_SFLOAT; // stand for vec3 of 32bits floats
		attributeDescriptions[attribute].offset = offsetof(Vertex, color);

		attribute++;
		attributeDescriptions[attribute].binding = 0;
		attributeDescriptions[attribute].location = 2;
		attributeDescriptions[attribute].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[attribute].offset = offsetof(Vertex, texCoord);

		attribute++;
		attributeDescriptions[attribute].binding = 0;
		attributeDescriptions[attribute].location = 3;
		attributeDescriptions[attribute].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[attribute].offset = offsetof(Vertex, normal);

		return attributeDescriptions;
	}

	bool operator==(const Vertex& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
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