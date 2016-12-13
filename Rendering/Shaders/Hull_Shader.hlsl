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
};

struct VS_CONTROL_POINT_OUTPUT
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;
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
	FUZ_COLLISION_POINT fuzPoint[MAX_FUZ_COUNT];
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

		// determine if the new triangle needs to be Fuzed
		// do sphere collision again
		//******************************************************************************************
		// Perform a sphere to triangle collision and Fuz it if so
		//******************************************************************************************
		bool Fuzed = false;

	//[fastopt]
	for (int i = 0; i < MAX_FUZ_PER_OBJECT; i++)
	{
		if (fuzIndex[i][0] < 0 || fuzIndex[i][0] > MAX_FUZ_COUNT)
		{
			continue;
		}

		if (fuzPoint[fuzIndex[i][0]].r_l_p_p.y < 0.0001)
		{
			continue;
		}

		// 1. test if sphere lies outside the triangle plane
		//		a. Translate problem so sphere is centered at origin
		float3 A = pos1.xyz - fuzPoint[fuzIndex[i][0]].worldPos.xyz;
			float3 B = pos2.xyz - fuzPoint[fuzIndex[i][0]].worldPos.xyz;
			float3 C = pos3.xyz - fuzPoint[fuzIndex[i][0]].worldPos.xyz;
			//		b. Compute a vector normal to triangle plane, normalize it
			float3 V = cross(B - A, C - A);
			float3 N = V / sqrt(dot(V, V));
			//		c. compute distance d of sphere center to triangle plane
			float d = abs(dot(A, N));

		if (d < fuzPoint[fuzIndex[i][0]].r_l_p_p.x) // continue checking for collision
		{
			// 2. test if sphere lies outside of a triangle vertex
			float aa = dot(A, A);
			float ab = dot(A, B);
			float ac = dot(A, C);
			float bb = dot(B, B);
			float bc = dot(B, C);
			float cc = dot(C, C);
			float rr = fuzPoint[fuzIndex[i][0]].r_l_p_p.x * fuzPoint[fuzIndex[i][0]].r_l_p_p.x;

			if( false == ((aa > rr) && (ab > aa) && (ac > aa) || (bb > rr) && (ab > bb) && 
				(bc > bb) || (cc > rr) && (ac > cc) && (bc > cc)))
			{
				// 3. test if sphere lies outside a triangle edge
				float3 AB = B - A;
				float3 BC = C - B;
				float3 CA = A - C;
				float d1 = dot(A, AB);
				float e1 = dot(AB, AB);
				float d2 = dot(B, BC);
				float e2 = dot(BC, BC);
				float d3 = dot(C, CA);
				float e3 = dot(CA, CA);
				float3 Q1 = A * e1 - d1 * AB;
				float3 QC = C * e1 - Q1;
				float3 Q2 = B * e2 - d2 * BC;
				float3 QA = A * e2 - Q2;
				float3 Q3 = C * e3 - d3 * CA;
				float3 QB = B * e3 - Q3;

				float r = fuzPoint[fuzIndex[i][0]].r_l_p_p.x;

				bool separated1 = (dot(Q1, Q1) > r * r * e1 * e1 && dot(Q1, QC) > 0);
				bool separated2 = (dot(Q2, Q2) > r * r * e2 * e2 && dot(Q2, QA) > 0);
				bool separated3 = (dot(Q3, Q3) > r * r * e3 * e3 && dot(Q3, QB) > 0);
				bool separated = (separated1 || separated2 || separated3);

				if( false == separated)
				{
					Fuzed = true;
					break;
				}
			}
		}
	}
	// End of sphere to triangle test
	//*******************************************************************************************

	Output.bTesselated = Fuzed;

	if(Fuzed)
	{
		//tess = 4.0f;// colorRGBA.w; 
		// Insert code to compute Output here
		// Tesselation for FUZ should be limited so that tesselation doesn't 
		//		split any edges to be shorter than .25 
		//			- Geometry may have to be altered so that edge lengths are also not too long

		Output.EdgeTessFactor[0] = clamp(length(edge1) * 4.0f, 1.0f, 64.0f);
		Output.EdgeTessFactor[1] = clamp(length(edge2) * 4.0f, 1.0f, 64.0f);
		Output.EdgeTessFactor[2] = clamp(length(edge3) * 4.0f, 1.0f, 64.0f);
		Output.InsideTessFactor = min(Output.EdgeTessFactor[0], max(Output.EdgeTessFactor[1], Output.EdgeTessFactor[2]));		
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
[partitioning("fractional_odd")] //fractional_odd //fractional_even // integer
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

	return output;
}

