#version 330 core

uniform sampler2D texture_screen;
uniform sampler2D texture_blurred;

in vec2 out_tex;
layout (location = 0) out vec4 color;

void main()
{
	vec4 tmp_blur = texture(texture_blurred,out_tex);

	// make the bright brighter and the dark darker
	tmp_blur *= 2.0;
	tmp_blur *= tmp_blur;
	///

	color = texture(texture_screen,out_tex)+tmp_blur;
}
