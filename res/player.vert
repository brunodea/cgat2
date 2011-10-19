#version 330

uniform mat4 projection;
uniform mat4 modelview;

in vec4 in_Position;
in vec4 in_Color;

out vec4 ex_Color;

void main(void) 
{
    gl_Position = projection*(modelview*in_Position);
	ex_Color = in_Color;
}
  