#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float radius : RADIUS;
	float lifetime : LIFETIME;
};

struct OUTPUT_VERTEX
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float radius : RADIUS;
	float lifetime : LIFETIME;
};

//cbuffer OBJECT : register (b0)
//{
//	float4x4 Local_worldMatrix;
//};

cbuffer SCENE : register (b1)
{
	float4x4 Scene_worldMatrix;
	float4x4 projectionMatrix;	
};

OUTPUT_VERTEX main( INPUT_VERTEX fromVertexBuffer )
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	sendToRasterizer = fromVertexBuffer;	

	return sendToRasterizer;
}