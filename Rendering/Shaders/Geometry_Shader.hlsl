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
	bool bFuzed : BFUZED;
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



	//FUZ_COLLISION_POINT Test_FuzSeed;
	//Test_FuzSeed.worldPos = float4(colorRGBA.x,0, colorRGBA.z, 1);
	//Test_FuzSeed.color = float4(1, 0, 0, 1);
	//Test_FuzSeed.r_l_p_p.x = 10;
	//Test_FuzSeed.r_l_p_p.y = 10.0f;

	float4 pos1 = float4(input[0].position, 1);
		float4 pos2 = float4(input[1].position, 1);
		float4 pos3 = float4(input[2].position, 1);

		float4 edge1 = pos2 - pos1;
		float4 edge2 = pos3 - pos1;
		float4 edge3 = pos3 - pos2;

		// determine if the new triangle needs to be Fuzed
		// do sphere collision again
		//******************************************************************************************
		// Perform a sphere to triangle collision and Fuz it if so
		//******************************************************************************************
		bool Fuzed = false;
	float4 FuzColor = float4(0,0,0,0);

		if(input[0].bFuzed == true)
		{
			//[fastopt]
		for (int i = 0; i < MAX_FUZ_PER_OBJECT; i++)
			{

				if(fuzIndex[i][0] < 0 || fuzIndex[i][0] > MAX_FUZ_COUNT)
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

					if( false == ((aa > rr) && (ab > aa) && (ac > aa) || (bb > rr) && (ab > bb) && (bc > bb) || (cc > rr) && (ac > cc) && (bc > cc)))
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
							FuzColor = fuzPoint[fuzIndex[i][0]].color;
							break;
						}
					}
				}
			}
		}
		// End of sphere to triangle test
		//*******************************************************************************************


		if(!Fuzed)
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
		else
		{
			// make the four verts
			GS_OUTPUT vert[4];

			//vert[0].projectedCoordinate = input[0].projectedCoordinate;
			vert[0].position = input[0].position;
			vert[0].UVs = input[0].UVs;
			vert[0].normal = input[0].normal;
			vert[0].colorRGBA = float4(0,0,0,1) + FuzColor;

			//vert[1].projectedCoordinate = input[1].projectedCoordinate;
			vert[1].position = input[1].position;
			vert[1].UVs = input[1].UVs;
			vert[1].normal = input[1].normal;
			vert[1].colorRGBA = float4(1,1,1,1) + FuzColor;

			//vert[2].projectedCoordinate = input[2].projectedCoordinate;
			vert[2].position = input[2].position;
			vert[2].UVs = input[2].UVs;
			vert[2].normal = input[2].normal;
			vert[2].colorRGBA = float4(.5,.5,.5,1) + FuzColor;

			float3 avgPos = 0.333f * (vert[0].position + vert[1].position + vert[2].position);
				float3 avgNormal = 0.333f * (vert[0].normal + vert[1].normal + vert[2].normal);
				float3 newPos = avgPos + 1.5f * avgNormal;

				vert[3].position = newPos;


			vert[3].UVs = 0.333f * (vert[0].UVs + vert[1].UVs + vert[2].UVs);
			vert[3].normal = avgNormal;
			vert[3].colorRGBA = FuzColor; //float4(4,0,0,1);

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

}
