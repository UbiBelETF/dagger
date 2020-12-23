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
layout (location = 9) in float ai_Rotation;
layout (location = 10) in float ai_IsUI;
layout (location = 11) in float ai_Grayscale;

uniform mat4 u_Projection;
uniform mat4 u_Viewport;
uniform mat4 u_Camera;

out highp vec2 v_TextureCoord;
out highp vec2 v_SubTexSize;
out highp vec2 v_SubTexOrigin;
out highp vec4 v_QuadColor;
out highp float v_Grayscale;

void main()
{
	float radianRotation = 0.0174533 * ai_Rotation;
	float cosRotation = cos(radianRotation);
	float sinRotation = sin(radianRotation);

	v_TextureCoord = a_TextureCoord;
	v_SubTexSize = ai_SubTexSize;
	v_SubTexOrigin = ai_SubTexOrigin;
	v_QuadColor = ai_QuadColor;
	v_Grayscale = ai_Grayscale;

	vec2 recenteredVertexPosition = a_VertexPosition.xy + ai_QuadPivot.xy;
	recenteredVertexPosition.x *= ai_ImageDimensions.x * ai_Scale.x;
	recenteredVertexPosition.y *= ai_ImageDimensions.y * ai_Scale.y;

	vec2 rotatedVertexPosition = vec2(
		cosRotation * recenteredVertexPosition.x - sinRotation * recenteredVertexPosition.y, 
		sinRotation * recenteredVertexPosition.x + cosRotation * recenteredVertexPosition.y);

	vec4 position = vec4(rotatedVertexPosition + ai_QuadPosition.xy, -ai_QuadPosition.z, 1.0f);

	if(ai_IsUI < 0.5f)
		gl_Position = u_Projection * u_Viewport * u_Camera * position;
	else
		gl_Position = u_Projection * u_Viewport * position;
}
