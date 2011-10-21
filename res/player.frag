#version 330

uniform vec3 sunlightpos;

in  vec4 ex_Color;
out vec4 out_Color;

void main(void)
{
	float value = sunlightpos.x;

	out_Color = ex_Color;
}