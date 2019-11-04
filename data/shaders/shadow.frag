#version 450

layout(binding = 1) uniform ShadowUniform {
	mat4 light;	// sun's MVP matrix
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


float lambert(vec3 normal, vec3 light, vec3 position) {
	return max(0.0, dot(normalize(normal), normalize(light - position)));
}

float shadow(vec4 lightSpacePosition, vec3 lightDir) {
	// Needed in case of a perspective projection
	vec3 projCoords = lightSpacePosition.xyz / lightSpacePosition.w;
	// Range is [-1,1], change it to [0,1]
	projCoords = (projCoords * 0.5) + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	// prevent shadow acne by adding bias
	float bias = 0.00008;
	// check wether current frag in in shadw or not
	float shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
	return shadow;
}

void main() {
	float inShadow = shadow(lightSpaceFragPos, subo.lightWorldPos - fragWorldPos);

	//float cos_theta = lambert(fragNormal, lightWorldPos, fragWorldPos);

	vec4 texture_color = texture(texSampler, fragTexCoord);

	float shadowRatio = 0.9;

	outColor = texture_color * (1.0 - (shadowRatio * inShadow));// * cos_theta;
	//outColor = texture(texSampler, fragTexCoord);
}