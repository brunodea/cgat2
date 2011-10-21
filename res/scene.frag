#version 330

uniform sampler2D renderedTexture;
uniform float scopeDiameter;

in vec2 UV;
in vec4 position;
layout(location = 0) out vec4 out_Color;

void main(void) 
{
	out_Color = texture2D(renderedTexture,UV).rgba;

	float d = distance(UV,vec2(0.5f,0.5f));

	if(d > scopeDiameter)
	{
		ivec3 aux = ivec3(-4,0,4);

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

		out_Color = sum;
	}
	else if(d >= scopeDiameter*.96f || (UV.x <= 0.5f && UV.x >= 0.499f) || (UV.y <= 0.5f && UV.y >= 0.499f))
		out_Color = vec4(0,0,.2,1);
	else
		discard;
}
