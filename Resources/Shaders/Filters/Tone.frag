#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeColour;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

const vec3 white = vec3(1.0f, 1.0f, 1.0f);
const float exposure = 1.3f;

vec3 toneMap(vec3 colour) 
{
	return colour / (1.0 + colour);
}

void main() 
{
	vec3 textureColour = texture(samplerColour, inUv).rgb;
	outColour = vec4(toneMap(textureColour * exposure) / toneMap(white), 1.0f);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
