#version 330 core

layout (location = 0) in vec2 a_VertexPosition;
layout (location = 1) in vec2 a_TextureCoord;

layout (location = 2) in vec2 ai_SubTexSize;
layout (location = 3) in vec2 ai_SubTexOrigin;
layout (location = 4) in vec2 ai_ImageDimensions;

layout (location = 5) in vec3 ai_QuadPosition;
layout (location = 6) in vec2 ai_QuadPivot;
layout (location = 7) in vec4 ai_QuadColor;
layout (location = 8) in vec2 ai_Scale;

uniform mat4 u_Projection;
uniform mat4 u_Viewport;
uniform mat4 u_Camera;

out highp vec2 v_TextureCoord;
out highp vec2 v_SubTexSize;
out highp vec2 v_SubTexOrigin;
out highp vec4 v_QuadColor;

void main()
{
	v_TextureCoord = a_TextureCoord;
	v_SubTexSize = ai_SubTexSize;
	v_SubTexOrigin = ai_SubTexOrigin;

	v_QuadColor = ai_QuadColor;

	vec2 recenteredVertexPosition = a_VertexPosition.xy + ai_QuadPivot.xy;
	recenteredVertexPosition.x *= ai_ImageDimensions.x * ai_Scale.x;
	recenteredVertexPosition.y *= ai_ImageDimensions.y * ai_Scale.y;

	gl_Position = u_Projection * u_Viewport * u_Camera * 
		vec4(recenteredVertexPosition + ai_QuadPosition.xy, -ai_QuadPosition.z, 1.0f);
}
