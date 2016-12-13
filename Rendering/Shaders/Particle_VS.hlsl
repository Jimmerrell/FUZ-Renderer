#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float  size : PSIZE;
	float4 color : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 posL : SV_POSITION;
	float  size : PSIZE;
	float4 color : COLOR;
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

OUTPUT_VERTEX main(INPUT_VERTEX particle)
{
	OUTPUT_VERTEX toGS = (OUTPUT_VERTEX)0;

	toGS.posL = float4(particle.coordinate.xyz, 1.0f);
	toGS.posL.w = particle.coordinate.w;
	toGS.size = particle.size;
	toGS.color = particle.color;

	return toGS;
}