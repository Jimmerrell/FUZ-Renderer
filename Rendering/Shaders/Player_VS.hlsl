#pragma pack_matrix(row_major)
#define JOINT_MAX 50

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
	
	bool bFuzed : FUZED;
	float3 fuzColor : FUZ_COLOR;
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

cbuffer BINDPOS_JOINTS : register (b3)
{
	float4x4 BindPos[JOINT_MAX];
}

cbuffer INTERPOLATED_JOINTS : register (b4)
{
	float4x4 Interpolated[JOINT_MAX];
}

cbuffer UV_SCROLLING : register (b5)
{
	float4 uv_offsets;
};

cbuffer JOINT_FUZ_DATA : register (b6)
{
	float4 jointFuzData[JOINT_MAX];
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;
	
	uint index = fromVertexBuffer.influenceIndices.x;
	
	sendToRasterizer.bFuzed = false;

	if(jointFuzData[index].w > 0 )
	{
		sendToRasterizer.bFuzed = true;
		sendToRasterizer.fuzColor = jointFuzData[index].xyz;
	}

	float4 sum = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 v = float4(fromVertexBuffer.coordinate, 1.0f);

	float w = fromVertexBuffer.influencesWeights.x;
	float4x4 B = BindPos[index];
	float4x4 C = Interpolated[index];
	float4 Bv, CBv;
	Bv = mul(v, B);
	CBv = mul(Bv, C);

	float4 vec = float4(w * CBv.x, w * CBv.y, w * CBv.z, 1.0f);
	sum.x += vec.x;
	sum.y += vec.y;
	sum.z += vec.z;

	w = fromVertexBuffer.influencesWeights.y;
	if(w > 0.0f)
	{
		index = fromVertexBuffer.influenceIndices.y;
		B = BindPos[index];
		C = Interpolated[index];
		Bv = mul(v, B);
		CBv = mul(Bv, C);

		vec = float4(w * CBv.x, w * CBv.y, w * CBv.z, 1.0f);
		sum.x += vec.x;
		sum.y += vec.y;
		sum.z += vec.z;
	}

	w = fromVertexBuffer.influencesWeights.z;
	if(w > 0.0f)
	{
		index = fromVertexBuffer.influenceIndices.z;
		B = BindPos[index];
		C = Interpolated[index];
		Bv = mul(v, B);
		CBv = mul(Bv, C);

		vec = float4(w * CBv.x, w * CBv.y, w * CBv.z, 1.0f);
		sum.x += vec.x;
		sum.y += vec.y;
		sum.z += vec.z;
	}

	w = fromVertexBuffer.influencesWeights.w;
	if(w > 0.0f)
	{
		index = fromVertexBuffer.influenceIndices.w;
		B = BindPos[index];
		C = Interpolated[index];
		Bv = mul(v, B);
		CBv = mul(Bv, C);

		vec = float4(w * CBv.x, w * CBv.y, w * CBv.z, 1.0f);
		sum.x += vec.x;
		sum.y += vec.y;
		sum.z += vec.z;
	}

	float4 localH = sum;

	//localH = mul(localH, Local_worldMatrix);

	sendToRasterizer.position = localH.xyz;

	//localH = mul(localH, ViewMatrix);

	//localH = mul(localH, projectionMatrix);

	sendToRasterizer.UVs.x = fromVertexBuffer.texCoord.x + uv_offsets.x;
	sendToRasterizer.UVs.y = fromVertexBuffer.texCoord.y + uv_offsets.y;

	sendToRasterizer.normal = fromVertexBuffer.normal;
	float4 norm = float4(sendToRasterizer.normal, 1.0);
	//norm = mul(norm, Local_worldMatrix);
	sendToRasterizer.normal = norm.xyz;

	sendToRasterizer.projectedCoordinate = localH;

	sendToRasterizer.colorRGBA = colorRGBA;

	return sendToRasterizer;
}
