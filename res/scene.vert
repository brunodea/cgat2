#version 330

in vec3 pos;
in vec2 vertexUV;
out vec2 UV;
out vec4 position;

void main(void) 
{
	gl_Position = vec4(pos.xyz,1.0);
	UV = vertexUV;
	position = gl_Position;
}