#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TextureCoord;
in vec4 v_QuadColor;

out vec4 o_FragColor;

void main()
{
	
	vec4 tex = texture(u_Texture, v_TextureCoord);
	o_FragColor = tex * v_QuadColor;
}