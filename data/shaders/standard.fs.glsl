#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TextureCoord;
in vec4 v_QuadColor;

out vec4 o_FragColor;

void main()
{
	o_FragColor = texture(u_Texture, v_TextureCoord) * v_QuadColor;
}