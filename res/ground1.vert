#version 330

in vec4 in_Position;
in vec2 in_TexCoord;

smooth out vec2 texCoord;
out vec3 pos;

uniform mat4 projection;
uniform mat4 modelview;

void main(void) 
{
	gl_Position = projection*(modelview*in_Position);
	pos = gl_Position.xyz;
	texCoord = in_TexCoord;
}
  