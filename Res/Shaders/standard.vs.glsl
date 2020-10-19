#version 400 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TextureCoord;

layout (location = 2) in vec3 a_QuadPosition;
layout (location = 3) in vec2 a_QuadPivot;
layout (location = 4) in vec4 a_QuadColor;

layout (location = 5) in float a_Image;
layout (location = 6) in float a_Ratio;
layout (location = 7) in float a_Scale;

layout (location = 0) uniform mat4 u_Projection;

out vec2 v_TextureCoord;
out vec4 v_QuadColor;
flat out float v_Image;

void main()
{
	v_TextureCoord = a_TextureCoord;
	v_QuadColor = a_QuadColor;
	v_Image = a_Image;

	vec2 xy = a_Position.xy + a_QuadPosition.xy + a_QuadPivot.xy;
	xy.y *= a_Ratio;

	gl_Position = u_Projection * vec4(xy * a_Scale * 600, a_QuadPosition.z, 1.0f);
}
