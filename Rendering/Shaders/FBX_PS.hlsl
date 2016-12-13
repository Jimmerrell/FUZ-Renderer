texture2D baseTexture : register(t0); // first texture

SamplerState filters[2] : register(s0); // filter 0 using CLAMP, filter 1 using WRAP

struct INPUT_PIXEL
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;
	
	bool bFuzed : FUZED;
};

float4 main(INPUT_PIXEL input) : SV_TARGET
{
	float4 finalColor;

	float4 baseColor = baseTexture.Sample(filters[0], input.UVs.xy);

	if(baseColor.w < 0.001)
	{
		discard;
		//baseColor.w = 1.0f;
	}

	if(input.bFuzed)
	{
		baseColor = (baseColor * 0.1) + float4(.5, .5, .5, 1);
	}

	//if (input.bFuzed == false)
	//{
	//	discard;
	//}

	float4 AmbientLightColor = {0.15f, 0.15f, 0.15f, 1.0f};
	float4 PointLightColor_01 = {0.6f, 0.6f, 0.6f, 1.0f};
	float4 PointLightColor_02 = {0.6f, 0.6f, 0.6f, 1.0f};
	float4 PointLightColor_03 = {0.6f, 0.6f, 0.6f, 1.0f};
	float4 PointLightColor_04 = {0.6f, 0.6f, 0.6f, 1.0f};

	//float3 DirlightColor = {0.8f, 0.8f, 1.0f};
	//float3 lightDir = { 0.3f, -0.8f, 0.3f };

	//AmbientResult = lightColor * surfaceColor;
	float4 AmbientResult = AmbientLightColor * baseColor;

	//LightRatio = clamp(dot(-LightDir, surfacenormal));	
	//float lightRatio = saturate(dot(-lightDir, input.normal.xyz));

	// Directional Light result = lightRatio * LightColor * SurfaceColor
	//float4 DirResult = float4(saturate(lightRatio * DirlightColor * baseColor), 1.0f);

	float4 PL_01_Result;
	float4 PL_02_Result;
	float4 PL_03_Result;
	float4 PL_04_Result;

	float3 PL_01_Position = {100.0f, 500.0f, 100.0f};
		float3 PL_02_Position = {100.0f, 500.0f, -500.0f};
		float3 PL_03_Position = {-500.0f, 500.0f, -500.0f};
		float3 PL_04_Position = {-500.0f, 500.0f, 100.0f};

		float PL_Radius = 1500.0f;

	float3 PL_01_lightDir = normalize(PL_01_Position - input.position);
	float PL_01_lightRatio = saturate(dot(PL_01_lightDir, input.normal.xyz));
	float PL_01_Attenuation = 1.0 - saturate(length(PL_01_Position - input.position) / PL_Radius);
	PL_01_Result = saturate(PL_01_lightRatio * PointLightColor_01 * baseColor);
	PL_01_Result *= PL_01_Attenuation;

	float3 PL_02_lightDir = normalize(PL_02_Position - input.position);
	float PL_02_lightRatio = saturate(dot(PL_02_lightDir, input.normal.xyz));
	float PL_02_Attenuation = 1.0 - saturate(length(PL_02_Position - input.position) / PL_Radius);
	PL_02_Result = saturate(PL_02_lightRatio * PointLightColor_01 * baseColor);
	PL_02_Result *= PL_02_Attenuation;

	float3 PL_03_lightDir = normalize(PL_03_Position - input.position);
	float PL_03_lightRatio = saturate(dot(PL_03_lightDir, input.normal.xyz));
	float PL_03_Attenuation = 1.0 - saturate(length(PL_03_Position - input.position) / PL_Radius);
	PL_03_Result = saturate(PL_03_lightRatio * PointLightColor_01 * baseColor);
	PL_03_Result *= PL_03_Attenuation;

	float3 PL_04_lightDir = normalize(PL_04_Position - input.position);
	float PL_04_lightRatio = saturate(dot(PL_04_lightDir, input.normal.xyz));
	float PL_04_Attenuation = 1.0 - saturate(length(PL_04_Position - input.position) / PL_Radius);
	PL_04_Result = saturate(PL_04_lightRatio * PointLightColor_01 * baseColor);
	PL_04_Result *= PL_04_Attenuation;




	finalColor = AmbientResult + PL_01_Result + PL_02_Result + PL_03_Result + PL_04_Result;

	finalColor *= input.colorRGBA;

	return finalColor;


	//*************************************88
	// Original FBX pixel Shader
	//***************************************
	//	float4 baseColor = baseTexture.Sample(filters[0], input.UVs.xy);// * modulate; // get base color
	//	float4 finalColor = baseColor;  //float4(lerp(baseColor.rgb, detailColor.rgb, detailColor.a), baseColor.a);
	//	finalColor *= input.colorRGBA;
	//	return finalColor; // return a transition based on the detail alpha
}