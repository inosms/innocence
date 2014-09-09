#version 330 core

in vec3 out_normal;

out vec3 color;


void main()
{
	vec3 light_dir = normalize(vec3(0,0,1));
	float diff = clamp(dot(normalize(out_normal), normalize(light_dir)),0,1);
	float spec = pow(diff,50);
	float amb = 0.1;
	color = (spec+0.1*diff+amb) * vec3(1,1,1) + out_normal*0.1;
}