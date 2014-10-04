#version 330 core

uniform sampler2D tex;
uniform float texture_multiplier = 1.0;
uniform vec4 texture_color = vec4(1.0,1.0,1.0,1.0);
uniform float texture_u_offset = 0.0;

in vec2 out_tex;

out vec4 color;


void main()
{
	vec2 texture_coordinate = out_tex + vec2(texture_u_offset,0);
	color = texture(tex,texture_coordinate).zyxw * texture_multiplier * texture_color;
}
