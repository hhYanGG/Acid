#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(rgba16f, set = 0, binding = 0) uniform writeonly image2D writeColour;

layout(set = 0, binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

const vec4 lumcoeff = vec4(0.299f, 0.587f, 0.114f, 0.0f);
const float epsilon = 0.001f;

void main()
{
	vec4 tc_sampler[6];
	vec2 tc_offset[6];

	tc_offset[0] = vec2(-epsilon, -epsilon);
	tc_offset[1] = vec2(0.0f, -epsilon);
	tc_offset[2] = vec2(-epsilon, 0.0f);
	tc_offset[3] = vec2(epsilon, 0.0f);
	tc_offset[4] = vec2(0.0f, epsilon);
	tc_offset[5] = vec2(epsilon, epsilon);

	for(int i = 0; i < 6; i++) 
	{
		tc_sampler[i] = texture(samplerColour, inUv + tc_offset[i]);
	}

	vec4 sum = vec4(0.5) + (tc_sampler[0] + tc_sampler[1] + tc_sampler[2]) - (tc_sampler[3] + tc_sampler[4] + tc_sampler[5]);
	float lum = dot(sum, lumcoeff);

	outColour = vec4(lum, lum, lum, 1.0f);
	
	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
