#version 330 core

uniform sampler2D tex;

in vec3 out_normal;
in vec2 out_tex;

out vec3 color;


void main()
{
	vec3 light_dir = normalize(vec3(0,0,1));
	float diff = clamp(dot(normalize(out_normal), normalize(light_dir)),0,1);
	float spec = pow(diff,50);
	float amb = 0.1;
	//color = (spec+0.1*diff+amb) * vec3(1,1,1) + out_normal*0.1;
	color = vec3(texture(tex,out_tex).zyx);
}