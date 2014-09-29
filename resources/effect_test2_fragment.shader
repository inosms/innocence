#version 330 core

uniform sampler2D texture_lastEffect;

in vec2 out_tex;

layout (location = 0) out vec4 color;


void main()
{
	color += texture(texture_lastEffect,vec2(out_tex.x,out_tex.y+0.0021));
	color += texture(texture_lastEffect,vec2(out_tex.x,out_tex.y+-0.0021));
	color += texture(texture_lastEffect,vec2(out_tex.x,out_tex.y+0.0014));
	color += texture(texture_lastEffect,vec2(out_tex.x,out_tex.y+0.0007));
	color += texture(texture_lastEffect,vec2(out_tex.x,out_tex.y+-0.0007));
	color += texture(texture_lastEffect,vec2out_tex.x,out_tex.y+-0.0014));
	color += texture(texture_lastEffect,vec2(out_tex.x,out_tex.y));
	color = color * 0.14;
	color.a = 1;
	/*

	float tmp_width = 0.0007;
	float tmp_radius = 3;
	float tmp_count = 7;//tmp_radius + tmp_radius + 1;
	float tmp_divide = 0.14;//1.0/tmp_count;
	color = vec4(0,0,0,1);
	for( int i = 0; i <= tmp_count; i++)
	{
		float tmp_diff = float(i) - tmp_radius;
		tmp_diff = tmp_diff * tmp_width;
		color += tmp_divide*texture(texture_lastEffect,vec2(out_tex.x,tmp_diff+out_tex.y));
	}
	color.a = 1;
	*/
}
