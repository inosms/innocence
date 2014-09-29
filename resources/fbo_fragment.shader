#version 330 core

uniform sampler2D tex;

in vec2 out_tex;

out vec4 color;


void main()
{
	color = texture(tex,out_tex);
}
