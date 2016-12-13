texture2D baseTexture : register(t0); //foreground or fill

texture2D secondTexture : register(t1); //gradient

texture2D thirdTexture : register(t2); //not being used

SamplerState filters : register(s0);

struct INPUT_PIXEL
{
	float2 texCoord : TEXCOORD0;
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float gradient : TEXCOORD1; //is input.gradient any value passed through?
};

float4 main(INPUT_PIXEL input) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters, input.texCoord); //fill
	float4 gradientColor = secondTexture.Sample(filters, input.texCoord); //gradient
	if(baseColor.w < 0.000001f)
		discard;
	if(gradientColor.w < 0.000001f)
		discard;
	float gradient = input.gradient;
	if(gradient > 1.0f)
		gradient = 1.0f;
	if(gradient < 0.0f)
		gradient = 0.00001f;

	float difference = ( (1 - gradient) - gradientColor.x );
	if( difference > 0.015f )
	{
		baseColor.w = 0.0f;//((gradientColor.x) - (1-input.gradient));//(gradientColor.x  - input.gradient);
	}
	else if(difference > 0.013f)
	{
		baseColor.w = 0.2f;//((gradientColor.x) - (1-input.gradient))/2;
	}
	else if(difference > 0.011f)
	{
		baseColor.w = 0.4f;//((gradientColor.x) - (1-input.gradient))/2;
	}
	else if(difference > 0.009f)
	{
		baseColor.w = 0.6f;//((gradientColor.x) - (1-input.gradient))/2;
	}
	else if(difference > 0.007)
	{
		baseColor.w = 0.7f;//((gradientColor.x) - (1-input.gradient))/2;
	}

	baseColor = baseColor  * input.colorOut;
	//float4 finalColor = baseColor;
	return baseColor;

}