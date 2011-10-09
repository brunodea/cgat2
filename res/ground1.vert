#version 330

in vec2 in_TexCoord;
in vec4 in_Position;

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
  