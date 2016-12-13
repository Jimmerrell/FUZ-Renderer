#pragma pack_matrix(row_major)

#include "../ShaderShared.h"
//#define TESSELATION true


struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	uint4 influenceIndices : BLENDINDICES;
	float4 influencesWeights : BLENDWEIGHT;
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 colorRGBA : COLOR;
	float3 position : POSITION;
	float3 UVs : TEXTURECOORD;
	float3 normal : NORMAL;
};

struct FUZ_COLLISION_POINT
{
	float3 worldPos;
	float3 color;
	float radius;
	float lifeTime;
};

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

cbuffer UV_SCROLLING : register (b5)
{
	float4 uv_offsets;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
	if(TESSELATION)
	{
		OUTPUT_VERTEX toHullShader = (OUTPUT_VERTEX)0;
		toHullShader.projectedCoordinate = float4(fromVertexBuffer.coordinate, 1);
		toHullShader.colorRGBA = colorRGBA;
		toHullShader.position = fromVertexBuffer.coordinate;
		toHullShader.UVs.x = fromVertexBuffer.texCoord.x + uv_offsets.x;
		toHullShader.UVs.y = fromVertexBuffer.texCoord.y + uv_offsets.y;
		toHullShader.normal = fromVertexBuffer.normal;

		return toHullShader;
	}
	else
	{
		OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
		
		float4 localH = float4(fromVertexBuffer.coordinate, 1);
		
		localH = mul(localH, Local_worldMatrix);
		
		sendToRasterizer.position = localH.xyz;
		
		localH = mul(localH, ViewMatrix);
		
		localH = mul(localH, projectionMatrix);
		
		sendToRasterizer.UVs.x = fromVertexBuffer.texCoord.x + uv_offsets.x;
		sendToRasterizer.UVs.y = fromVertexBuffer.texCoord.y + uv_offsets.y;
		
		sendToRasterizer.normal = fromVertexBuffer.normal;
		sendToRasterizer.normal = float3(mul(float4(sendToRasterizer.normal.xyz, 1), Local_worldMatrix).xyz);
		
		sendToRasterizer.projectedCoordinate = localH;
		
		sendToRasterizer.colorRGBA = colorRGBA;
		
		return sendToRasterizer;
	}
}