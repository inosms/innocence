#version 330 core

uniform mat4 modelview;
uniform mat4 projection;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 3) in vec2 tex;

out vec2 out_tex;

// just a VERY BASIC shader
void main()
{
	gl_Position = projection * modelview * vec4(vertexPosition_modelspace,1.0);
	out_tex = tex;
}
