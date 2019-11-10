#version 450

layout(binding = 1) uniform ShadowUniform {
	mat4 light;	// sun's VP matrix
	vec3 viewWorldPos;
	vec3 lightWorldPos;
} subo;

layout(binding = 2) uniform sampler2D texSampler;
layout(binding = 3) uniform sampler2D shadowMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragWorldPos;
layout(location = 3) in vec4 lightSpaceFragPos;

layout(location = 0) out vec4 outColor;

float shadow(vec4 lightSpacePosition, sampler2D shadowMap) {
	// Needed in case of a perspective projection
	vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	// frag to light distance of the current fragment (Z is already on [0,1] range)
	float currentDepth = projCoords.z;
	// Range is [-1,1], change it to [0,1]
	projCoords = (projCoords * 0.5) + 0.5;
	// shadowmap depth
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// prevent shadow acne by adding bias
	float bias = 0.005;
	// check wether current frag in in shadow or not
	float shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
	return shadow;
}

void main() {
	float isInShadow = shadow(lightSpaceFragPos, shadowMap);

	vec4 color = texture(texSampler, fragTexCoord);

	vec4 ambient = 0.1 * color;

	outColor = color * (ambient + (1.0 - isInShadow));
}