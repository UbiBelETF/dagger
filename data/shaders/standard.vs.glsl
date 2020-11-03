#version 330 core

layout (location = 0) in vec2 a_VertexPosition;
layout (location = 1) in vec2 a_TextureCoord;

layout (location = 2) in vec3  ai_QuadPosition;
layout (location = 3) in vec2  ai_QuadPivot;
layout (location = 4) in vec4  ai_QuadColor;
layout (location = 5) in float ai_Ratio;
layout (location = 6) in vec2  ai_Scale;
layout (location = 7) in vec2  x_UnusedTexture;

uniform mat4 u_Projection;
uniform mat4 u_CameraView;

out vec2 v_TextureCoord;
out vec4 v_QuadColor;

void main()
{
	v_TextureCoord = a_TextureCoord;
	v_QuadColor = ai_QuadColor;

	vec2 pos = (a_VertexPosition.xy + ai_QuadPivot.xy);
	pos.x *= ai_Scale.x;
	pos.y *= ai_Scale.y;

	vec2 xy = ai_QuadPosition.xy + pos;
	xy.y *= ai_Ratio;

	gl_Position = u_Projection * u_CameraView * vec4(xy * 140.0f, ai_QuadPosition.z, 1.0f);
}
