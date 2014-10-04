#version 330 core

uniform sampler2D texture_screen;
uniform float screen_height;

in vec2 out_tex;
layout (location = 0) out vec4 color;

void main()
{
	float tmp_pixelHeight = float(1)/float(screen_height);
	color = vec4(0,0,0,0);
	color += texture(texture_screen,vec2(out_tex.x,tmp_pixelHeight*3.0+out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,-tmp_pixelHeight*3.0+out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,tmp_pixelHeight*2.0+out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,-tmp_pixelHeight*2.0+out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,tmp_pixelHeight+out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,-tmp_pixelHeight+out_tex.y));
	color += texture(texture_screen,vec2(out_tex.x,out_tex.y));
	color = color * (1.0/7.0);
	color.a = 1;
}
