#pragma pack_matrix(row_major)

struct GSOutput
{
	float2 UV	: TEXCOORD0;
	float4 pos : SV_POSITION;
	float4 colorOut : COLOR;
};

cbuffer SCENE : register (b1)
{
	float4x4 Scene_worldMatrix;
	float4x4 projectionMatrix;
};

struct GSInput
{
	float4 Pos : SV_POSITION;
	float  Size : PSIZE;
	float4 Color : COLOR;
};


[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream< GSOutput > output)
{
	float halfwidth = input[0].Size / 2.0;
	float halfheight = input[0].Size / 2.0;
	float rot = input[0].Pos.w;
	float4x4 mVP = mul(Scene_worldMatrix, projectionMatrix);

	float4 tempPOS = float4(input[0].Pos.xyz, 1.0f);

	tempPOS = mul(tempPOS, mVP);

	GSOutput verts[4] =
	{
		// UVs		  VERTEX position															   Color
		float2(1, 0), float4(tempPOS.x - halfwidth, tempPOS.y + halfheight, tempPOS.z, tempPOS.w), input[0].Color,
		float2(1, 1), float4(tempPOS.x + halfwidth, tempPOS.y + halfheight, tempPOS.z, tempPOS.w), input[0].Color,
		float2(0, 0), float4(tempPOS.x - halfwidth, tempPOS.y - halfheight, tempPOS.z, tempPOS.w), input[0].Color,
		float2(0, 1), float4(tempPOS.x + halfwidth, tempPOS.y - halfheight, tempPOS.z, tempPOS.w), input[0].Color
	};						

	for (uint i = 0; i < 4; i++)
	{
		float x = verts[i].pos.x;
		float y = verts[i].pos.y;
		verts[i].pos.x = (x - tempPOS.x) * cos(rot) - (y - tempPOS.y) * sin(rot) + tempPOS.x;
		verts[i].pos.y = (y - tempPOS.y) * cos(rot) + (x - tempPOS.x) * sin(rot) + tempPOS.y;
		output.Append(verts[i]);
	}

	output.RestartStrip();
}