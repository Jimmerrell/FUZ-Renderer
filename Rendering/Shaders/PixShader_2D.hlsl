texture2D baseTexture : register(t0); // first texture

texture2D secondTexture : register(t1);

texture2D thirdTexture : register(t2);

SamplerState filters : register(s0);

struct INPUT_PIXEL
{
	float2 texCoord : TEXCOORD0;
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float gradient : TEXCOORD1;
};

float4 main(INPUT_PIXEL input) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters, input.texCoord); // get base color
	if (baseColor.w < 0.001)
	{
		discard;
	}
	baseColor = baseColor  * input.colorOut;
	float4 finalColor = baseColor;
		return finalColor;

}