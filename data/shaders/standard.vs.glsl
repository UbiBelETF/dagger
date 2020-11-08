#version 330 core

layout (location = 0) in vec2 a_VertexPosition;
layout (location = 1) in vec2 a_TextureCoord;

layout (location = 2) in vec3  ai_QuadPosition;
layout (location = 3) in vec2  ai_QuadPivot;
layout (location = 4) in vec4  ai_QuadColor;
layout (location = 5) in vec2  ai_Size;
layout (location = 6) in vec2  ai_Scale;

uniform mat4 u_Projection;
uniform mat4 u_Viewport;
uniform mat4 u_Camera;

out vec2 v_TextureCoord;
out vec4 v_QuadColor;

void main()
{
	v_TextureCoord = a_TextureCoord;
	v_QuadColor = ai_QuadColor;

	vec2 recenteredVertexPosition = a_VertexPosition.xy + ai_QuadPivot.xy;
	recenteredVertexPosition.x *= ai_Size.x * ai_Scale.x;
	recenteredVertexPosition.y *= ai_Size.y * ai_Scale.y;

	gl_Position = u_Projection * u_Viewport * u_Camera * 
		vec4(recenteredVertexPosition  + ai_QuadPosition.xy, -ai_QuadPosition.z, 1.0f);
}
