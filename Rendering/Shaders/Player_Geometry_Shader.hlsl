#pragma pack_matrix(row_major)

//#define FUZ_COUNT 10
#include "../ShaderShared.h"

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

struct GS_OUTPUT
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;
	
	bool bFuzed : FUZED;
};

[maxvertexcount(6)]
void main(
	triangle DS_OUTPUT input[3] : SV_POSITION, 
	inout TriangleStream< GS_OUTPUT > output
	)
{

	float4 pos1 = float4(input[0].position, 1);
		float4 pos2 = float4(input[1].position, 1);
		float4 pos3 = float4(input[2].position, 1);

		float4 edge1 = pos2 - pos1;
		float4 edge2 = pos3 - pos1;
		float4 edge3 = pos3 - pos2;

		bool Fuzed = false;
		float4 FuzColor = float4(0,0,0,0);

		if(input[0].bFuzed || input[1].bFuzed || input[2].bFuzed)
		{
			// make the four verts
			GS_OUTPUT vert[4];

			//vert[0].projectedCoordinate = input[0].projectedCoordinate;
			vert[0].position = input[0].position;
			vert[0].UVs = input[0].UVs;
			vert[0].normal = input[0].normal;
			vert[0].colorRGBA = float4(0,0,0,1) + float4(input[0].fuzColor, 1);

			//vert[1].projectedCoordinate = input[1].projectedCoordinate;
			vert[1].position = input[1].position;
			vert[1].UVs = input[1].UVs;
			vert[1].normal = input[1].normal;
			vert[1].colorRGBA = float4(1,1,1,1) + float4(input[0].fuzColor, 1);

			//vert[2].projectedCoordinate = input[2].projectedCoordinate;
			vert[2].position = input[2].position;
			vert[2].UVs = input[2].UVs;
			vert[2].normal = input[2].normal;
			vert[2].colorRGBA = float4(.5,.5,.5,1) +float4(input[0].fuzColor, 1);

			
			float3 avgPos = 0.333f * (vert[0].position + vert[1].position + vert[2].position);
				//float3 avgNormal = 0.333f * (vert[0].normal + vert[1].normal + vert[2].normal);
				//float3 newNormal = cross(edge1.xyz, edge2.xyz);

			// get two of the edges and cross them to get the normal in world space
			float3 ba = vert[1].position - vert[0].position;
			float3 ca = vert[2].position - vert[0].position;
			float3 newNormal = cross(normalize(ca), normalize(ba));
			
			float3 newPos = avgPos + 0.5f * newNormal; //avgNormal;

			vert[3].position = newPos;

			vert[3].UVs = 0.333f * (vert[0].UVs + vert[1].UVs + vert[2].UVs);
			vert[3].normal = newNormal; //avgNormal;
			vert[3].colorRGBA = float4(input[0].fuzColor, 1); //FuzColor; //float4(4,0,0,1);


			for(uint i = 0; i < 4; i++)
			{
				float4 localH = float4(vert[i].position, 1);
					localH = mul(localH, ViewMatrix);			
				localH = mul(localH, projectionMatrix);
				vert[i].projectedCoordinate = localH;

				vert[i].bFuzed = true;
			}


			// append the verts
			output.Append(vert[0]);
			output.Append(vert[1]);
			output.Append(vert[2]);
			output.Append(vert[3]);
			output.Append(vert[0]);
			output.Append(vert[1]);

		}
		else //(!Fuzed)
		{
			for (uint i = 0; i < 3; i++)
			{
				GS_OUTPUT element;

				element.bFuzed = false;
				//element.projectedCoordinate = input[i].projectedCoordinate;
				element.position = input[i].position;
				element.colorRGBA = colorRGBA;
				element.UVs = input[i].UVs;
				element.normal = input[i].normal;

				float4 localH = input[i].projectedCoordinate;
					localH = mul(localH, ViewMatrix);			
				localH = mul(localH, projectionMatrix);
				element.projectedCoordinate = localH;

				output.Append(element);
			}
		}
}