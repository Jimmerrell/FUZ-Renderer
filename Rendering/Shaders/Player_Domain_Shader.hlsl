#pragma pack_matrix(row_major)

struct HS_OUTPUT
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;
	
	bool bFuzed : FUZED;
	float3 fuzColor : FUZ_COLOR;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
	bool bTesselated : TESSELATED;
};

struct FUZ_COLLISION_POINT
{
	float4 worldPos;
	float4 color;
	float4 r_l_p_p; // radius, lifetime, padding, padding
};

//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------
cbuffer OBJECT : register (b0)
{
	float4x4 Local_worldMatrix;
};

cbuffer SCENE : register (b1)
{
	float4x4 ViewMatrix;
	float4x4 projectionMatrix;
};

cbuffer TRANSPARENCY : register (b2)
{
	float4 colorRGBA;
};

cbuffer FUZ_DATA : register (b3)
{
	FUZ_COLLISION_POINT fuzPoint[1];
};

struct DS_OUTPUT
{
    float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;
	
	bool bFuzed : FUZED;
	float3 fuzColor : FUZ_COLOR;
};



#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT output;

	// Interpolate position
	float4 localH = float4(patch[0].position*domain.x+patch[1].position*domain.y+patch[2].position*domain.z, 1);

	localH = mul(localH, Local_worldMatrix);

	output.position = localH.xyz;

	output.projectedCoordinate = localH;

	// Interpolate and normalize normal
	output.normal = normalize(patch[0].normal*domain.x+patch[1].normal*domain.y+patch[2].normal*domain.z);

	// Interpolate texture coordinate
	output.UVs = patch[0].UVs*domain.x+patch[1].UVs*domain.y+patch[2].UVs*domain.z;

	//output.normal = float4(mul(float4(output.normal.xyz, 0), Local_worldMatrix)).xyz;	

	output.colorRGBA = float4(1,1,1,1);//colorRGBA;

	output.bFuzed = patch[0].bFuzed;

	output.fuzColor = patch[0].fuzColor;

	return output;
}
