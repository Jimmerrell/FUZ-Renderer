texture2D baseTexture : register(t0); // first texture

SamplerState filters[1] : register(s0);

struct INPUT_PIXEL
{
	float2 texCoord : TEXCOORD0;
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float gradient : TEXCOORD1;
};

float4 main(INPUT_PIXEL input) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.texCoord); // get base color
		if (baseColor.w <= 1.0f - input.gradient)//f_Color.r<=1.0f-Health
		{
			discard;
		}
		input.colorOut.w = 1.0f;
		baseColor.r = 0.0f;
		baseColor.g = 0.0f;
		baseColor.b = 0.0f;
		baseColor.w = 1.0f;//baseColor  * input.colorOut;
		float4 finalColor = baseColor;
			return finalColor;
}