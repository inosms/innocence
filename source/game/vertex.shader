#version 330 core

uniform mat4 modelview;
uniform mat4 projection;
uniform mat4 normalMat;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 col;
layout(location = 2) in vec3 normal;

out vec3 out_normal;

void main()
{
	gl_Position = projection * modelview * vec4(vertexPosition_modelspace,1.0);
	out_normal = (normalMat * vec4(normal,0.0)).xyz;
}