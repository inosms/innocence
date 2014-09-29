#version 330 core

uniform sampler2D tex;

in vec3 out_normal;
in vec2 out_tex;

layout (location = 0) out vec4 colorOut;
layout (location = 1) out vec4 normalOut;

void main()
{
	vec3 light_dir = normalize(vec3(1,1,0));
	float diff = clamp(dot(normalize(out_normal), normalize(light_dir)),0,1);
	float spec = pow(diff,50);
	float amb = 0.1;
	colorOut = (spec+0.4*diff+amb) * vec4(1,1,1,0);
	colorOut.a = 1;
	
	normalOut = vec4(out_normal,1);
}
