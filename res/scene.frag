#version 330

uniform sampler2D renderedTexture;
in vec2 UV;
in vec4 position;
layout(location = 0) out vec3 out_Color;

void main(void) 
{
	out_Color = texture2D(renderedTexture,UV).rgb;

	float d = distance(position.xy,vec2(0.f,0.f));
	if(d > .6f)
	{
		ivec3 aux = ivec3(-2,0,2);

		vec4 sum = vec4(0,0,0,0);
		sum += textureOffset(renderedTexture,UV,aux.xx);
		sum += textureOffset(renderedTexture,UV,aux.yx);
		sum += textureOffset(renderedTexture,UV,aux.zx);

		sum += textureOffset(renderedTexture,UV,aux.xy);
		sum += textureOffset(renderedTexture,UV,aux.zy);

		sum += textureOffset(renderedTexture,UV,aux.zx);
		sum += textureOffset(renderedTexture,UV,aux.zy);
		sum += textureOffset(renderedTexture,UV,aux.zz);

		sum /= 8.f;

		out_Color = sum.rgb;
	}
	else if(d >= .58f)
		out_Color = vec3(0,0,0);
	else
		discard;
}
