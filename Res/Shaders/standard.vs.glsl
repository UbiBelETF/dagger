#version 400 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_gpu_shader_int64 : enable

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TextureCoord;
layout (location = 2) in vec3 a_QuadPosition;
layout (location = 3) in vec2 a_QuadPivot;
layout (location = 4) in vec4 a_QuadColor;
layout (location = 5) in float a_Image;

out vec2 v_TextureCoord;
out vec4 v_QuadColor;

flat out float v_Image;

void main()
{
	v_TextureCoord = a_TextureCoord;
	v_QuadColor = a_QuadColor;
	v_Image = a_Image;

	gl_Position = vec4((a_Position.xy + a_QuadPosition.xy + a_QuadPivot.xy) * 0.1f, a_QuadPosition.z * 0.1f, 1.0f);
}