textureCUBE baseTexture : register(t0); // first texture

//texture2D detailTexture : register(t1); // second texture

SamplerState filters[2] : register(s0); // filter 0 using CLAMP, filter 1 using WRAP

// Pixel shader performing multi-texturing with a detail texture on a second UV channel
// A simple optimization would be to pack both UV sets into a single register
float4 main(float3 baseUV : TEXCUBECOORD0, float4 modulate : COLOR) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], baseUV) * modulate; // get base color
	//	float4 detailColor = detailTexture.Sample(filters[1], detailUV); // get detail effect
	float4 finalColor = baseColor;  //float4(lerp(baseColor.rgb, detailColor.rgb, detailColor.a), baseColor.a);
	return finalColor; // return a transition based on the detail alpha
}
