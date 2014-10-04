#version 330 core

uniform sampler2D texture_screen;
uniform float screen_width;

in vec2 out_tex;
layout (location = 0) out vec4 color;

void main()
{
	float tmp_pixelWidth = float(1)/float(screen_width);
	color = vec4(0,0,0,0);
	color += texture(texture_screen,vec2(tmp_pixelWidth*3.0+out_tex.x,out_tex.y));
	color += texture(texture_screen,vec2(-tmp_pixelWidth*3.0+out_tex.x,out_tex.y));
	color += texture(texture_screen,vec2(tmp_pixelWidth*2.0+out_tex.x,out_tex.y));
	color += texture(texture_screen,vec2(-tmp_pixelWidth*2.0+out_tex.x,out_tex.y));
	color += texture(texture_screen,vec2(tmp_pixelWidth+out_tex.x,out_tex.y));
	color += texture(texture_screen,vec2(-tmp_pixelWidth+out_tex.x,out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,out_tex.y));
	color = color * (1.0/7.0);
	color.a = 1;
}
