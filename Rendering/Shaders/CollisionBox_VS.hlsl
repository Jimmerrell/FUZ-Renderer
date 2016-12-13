#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float3 coordinate : POSITION;
	float4 colorIN : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
};

cbuffer OBJECT : register (b0)
{
	float4x4 Local_worldMatrix;
};

cbuffer SCENE : register (b1)
{
	float4x4 Scene_worldMatrix;
	float4x4 projectionMatrix;	
};

OUTPUT_VERTEX main( INPUT_VERTEX fromVertexBuffer )
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	float4 localH = float4(fromVertexBuffer.coordinate, 1);

	localH = mul(localH, Local_worldMatrix);

	localH = mul(localH, Scene_worldMatrix);

	localH = mul(localH, projectionMatrix);

	sendToRasterizer.colorOut.x = fromVertexBuffer.colorIN.x;
	sendToRasterizer.colorOut.y = fromVertexBuffer.colorIN.y;
	sendToRasterizer.colorOut.z = fromVertexBuffer.colorIN.z;
	sendToRasterizer.colorOut.w = fromVertexBuffer.colorIN.w;

	sendToRasterizer.projectedCoordinate = localH;

	return sendToRasterizer;
}