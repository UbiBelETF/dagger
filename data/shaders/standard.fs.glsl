#version 330 core

uniform sampler2D u_Texture;

in highp vec2 v_TextureCoord;
in highp vec2 v_SubTexSize;
in highp vec2 v_SubTexOrigin;

in vec4 v_QuadColor;

out vec4 o_FragColor;

void main()
{	
	vec4 tex = texture(u_Texture, v_SubTexOrigin + v_TextureCoord * v_SubTexSize);
	o_FragColor = tex * v_QuadColor;
}