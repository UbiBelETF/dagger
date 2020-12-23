#version 330 core

uniform sampler2D u_Texture;

in highp vec2 v_TextureCoord;
in highp vec2 v_SubTexSize;
in highp vec2 v_SubTexOrigin;
in highp float v_Grayscale;

in vec4 v_QuadColor;

out vec4 o_FragColor;

void main()
{	
	vec4 tex = texture(u_Texture, v_SubTexOrigin + v_TextureCoord * v_SubTexSize);

	float average = (tex.r + tex.g + tex.b) / 3.0;
    vec4 gray = vec4(average, average, average, tex.a);

	vec4 mixd = tex * (1 - v_Grayscale) + gray * v_Grayscale;
	o_FragColor = mixd * v_QuadColor;
}