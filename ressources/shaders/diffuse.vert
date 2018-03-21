#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform
layout(binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
	vec3 source;
} ubo;

// Vertex input
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexcoord;
layout(location = 3) in vec3 normal;

// Vertex user output
layout(location = 0) out vec2 outTexcoord;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outSource;
layout(location = 3) out vec3 outPosition;

// Vertex built-in output
out gl_PerVertex {
	vec4 gl_Position;
};

void main() {
	mat4 mvp = ubo.proj * ubo.view * ubo.model;

	gl_Position = mvp * vec4(inPosition, 1);

	outNormal = vec3(ubo.model * vec4(normal, 1)); // Normaliser ou pas ?

	// source de lumière 
	outSource = ubo.source;

	outPosition = vec3(ubo.model * vec4(inPosition, 1.0));

	outTexcoord = inTexcoord;
}