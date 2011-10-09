#version 330

in vec4 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

uniform mat4 projection;
uniform mat4 modelview;

void main(void) 
{
	//gl_Position = projection*in_Position;
	//gl_Position = modelview*gl_Position;
	gl_Position = projection*(modelview*in_Position);
	ex_Color = in_Color;
}
  