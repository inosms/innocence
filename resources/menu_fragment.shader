#version 330 core

uniform sampler2D tex;
uniform float texture_multiplier = 1.0;
uniform vec3 texture_color = vec3(1.0,1.0,1.0);
uniform float texture_u_offset = 0.0;

in vec3 out_normal;
in vec2 out_tex;

out vec4 color;


void main()
{
	vec3 light_dir = normalize(vec3(0,0,1));
	float diff = clamp(dot(normalize(out_normal), normalize(light_dir)),0,1);
	float spec = pow(diff,50);
	float amb = 0.1;
	//color = (spec+0.1*diff+amb) * vec3(1,1,1) + out_normal*0.1;
	vec2 texture_coordinate = out_tex + vec2(texture_u_offset,0);
	color = texture(tex,texture_coordinate).zyxw * texture_multiplier * vec4(texture_color,1);
}