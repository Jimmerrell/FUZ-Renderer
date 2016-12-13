#pragma pack_matrix(row_major)

//#define FUZ_COUNT 10
#include "../ShaderShared.h"

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

struct VS_CONTROL_POINT_OUTPUT
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;

	bool bFuzed : FUZED;
	float3 fuzColor : FUZ_COLOR;
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

cbuffer PER_OBJECT_FUZ_DATA : register (b4)
{
	int4 fuzIndex[MAX_FUZ_PER_OBJECT];
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
	bool bTesselated : TESSELATED;
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	float4 pos1 = float4(ip[0].position, 1);
		float4 pos2 = float4(ip[1].position, 1);
		float4 pos3 = float4(ip[2].position, 1);

		float tess = 1.0f;

	// get edge centers and radius and check sphere collision

	pos1 = mul(pos1, Local_worldMatrix);
	pos2 = mul(pos2, Local_worldMatrix);
	pos3 = mul(pos3, Local_worldMatrix);

	float4 edge1 = pos2 - pos1;
		float4 edge2 = pos3 - pos1;
		float4 edge3 = pos3 - pos2;

		bool Fuzed = false;

	if(ip[0].bFuzed || ip[1].bFuzed || ip[2].bFuzed)
		Fuzed = true;


	Output.bTesselated = Fuzed;

	if(Fuzed)
	{
		//tess = 4.0f;// colorRGBA.w; 
		// Insert code to compute Output here
		// Tesselation for FUZ should be limited so that tesselation doesn't 
		//		split any edges to be shorter than .25 
		//			- Geometry may have to altered so that edge lengths are also not too long

		Output.EdgeTessFactor[0] = clamp(length(edge1) * 4.0f, 1.0f, 64.0f);
		Output.EdgeTessFactor[1] = clamp(length(edge2) * 4.0f, 1.0f, 64.0f);
		Output.EdgeTessFactor[2] = clamp(length(edge3) * 4.0f, 1.0f, 64.0f);
		Output.InsideTessFactor = min(Output.EdgeTessFactor[0], max(Output.EdgeTessFactor[1], Output.EdgeTessFactor[2]));//15; // e.g. could calculate dynamic tessellation factors instead		
	}
	else
	{
		Output.EdgeTessFactor[0] = 1.0f;
		Output.EdgeTessFactor[1] = 1.0f;
		Output.EdgeTessFactor[2] = 1.0f;
		Output.InsideTessFactor = 1.0f;
	}

	return Output;
}

[domain("tri")]
[partitioning("integer")] //fractional_odd
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_OUTPUT output;

	// Insert code to compute Output here
	output.projectedCoordinate = ip[i].projectedCoordinate;
	output.colorRGBA = ip[i].colorRGBA;
	output.position = ip[i].position;
	output.normal = ip[i].normal;
	output.UVs = ip[i].UVs;
	output.bFuzed = ip[i].bFuzed;
	output.fuzColor = ip[i].fuzColor;

	return output;
}

