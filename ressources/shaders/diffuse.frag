#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform
layout(binding = 1) uniform sampler2D texture0;

// Fragment user input
layout(location = 0) in vec2 inTexcoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inSource;
layout(location = 3) in vec3 inPosition;

// Fragment output
layout(location = 0) out vec4 fragment_color;

void main() {
	// direction du fragment vers la source de lumiere
	vec3 light = normalize(inSource - inPosition);

	float cos_theta = dot(inNormal, light);

    // utiliser la couleur
    fragment_color = vec4(1) * max(0, floor(cos_theta * 5) / 5);

    // utiliser la texture
    //fragment_color = texture(texture0, inTexcoord) * max(0, cos_theta);
}