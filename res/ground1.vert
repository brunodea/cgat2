#version 330

in vec4 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

uniform mat4 uniform_persp;
uniform mat4 uniform_lookat;

void main(void) 
{
	gl_Position = uniform_persp*in_Position;
	gl_Position = uniform_lookat*gl_Position;
	ex_Color = in_Color;
}
