#version 330

uniform mat4 persp;
uniform mat4 lookat;
in vec4 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

void main(void) 
{
	gl_Position = persp*in_Position;
	gl_Position = lookat*in_Position;
	ex_Color = in_Color;
}
