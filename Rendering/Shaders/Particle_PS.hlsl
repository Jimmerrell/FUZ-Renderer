texture2D baseTexture : register(t0); 

SamplerState filters : register(s0); 

struct INPUT_PIXEL
{
	float2 UV	: TEXCOORD0;
	float4 pos : SV_POSITION;
	float4 colorOut : COLOR;
};

float4 main(INPUT_PIXEL input) : SV_TARGET
{
	float4 color = (baseTexture.Sample(filters, input.UV) * input.colorOut);
	if(color.w < 0.001f)
		discard;

	return color;
}