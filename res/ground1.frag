#version 330

uniform sampler2D textureMap;

smooth in vec2 texCoord;
in vec3 pos;

out vec4 out_Color;

void main(void) 
{
	vec3 normal = texture(textureMap, texCoord).rgb;
	normal *= 2.0;
	normal -= 1.0;

	vec3 vetDirLuz = normalize(vec3(0,10,10));
	float intensity = max(dot(normal,vetDirLuz),0.0);

	out_Color = vec4(intensity,intensity,intensity,1.0);
};
