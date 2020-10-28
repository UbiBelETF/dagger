#version 400 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_gpu_shader_int64 : enable
#extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) in vec2 a_VertexPosition;
layout (location = 1) in vec2 a_TextureCoord;

layout (location = 2) in vec3 a_QuadPosition;
layout (location = 3) in vec2 a_QuadPivot;
layout (location = 4) in vec4 a_QuadColor;

layout (location = 5) in float a_Image;
layout (location = 6) in float a_Ratio;
layout (location = 7) in vec2 a_Scale;

layout (location = 0) uniform mat4 u_Projection;
layout (location = 5) uniform mat4 u_CameraView;

out vec2 v_TextureCoord;
out vec4 v_QuadColor;
flat out float v_Image;

void main()
{
	v_TextureCoord = a_TextureCoord;

	v_QuadColor = a_QuadColor;
	v_Image = a_Image;

	vec2 pos = (a_VertexPosition.xy + a_QuadPivot.xy);
	pos.x *= a_Scale.x;
	pos.y *= a_Scale.y;

	vec2 xy = a_QuadPosition.xy + pos;
	xy.y *= a_Ratio;

	gl_Position = u_Projection * u_CameraView * vec4(xy * 140.0f, a_QuadPosition.z, 1.0f);
}
