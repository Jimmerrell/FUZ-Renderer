#pragma pack_matrix(row_major)

struct GSInput
{
	float3 pos : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float radius : RADIUS;
	float lifetime : LIFETIME;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float rand_1_05(in float2 uv)
{
	float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233)*2.0)) * 43758.5453));
		return abs(noise.x + noise.y) * 0.5;
}

cbuffer SCENE : register (b1)
{
	float4x4 Scene_worldMatrix;
	float4x4 projectionMatrix;
};

[maxvertexcount(128)] // 6 per pyramid * 21
void main(point GSInput input[1], inout TriangleStream< GSOutput > output)
{

	float4x4 mVP = mul(Scene_worldMatrix, projectionMatrix);
		float4 masterPosition = float4(input[0].pos, 1.0f);
		//position = mul(position, mVP);

		float4 position;

		GSOutput verts[4];

	float fuzWidth = 0.5f;
	float halfWidth = 0.25f;
	float fuzHeight = 3.0f;

	GSOutput vertex;
	vertex.color = float4(input[0].color, 1.0f);

	//*********************************************
	// Fuz 1
	position = masterPosition;

	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 2
	position = masterPosition;

	position.x += fuzWidth;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.9f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 3
	position = masterPosition;

	position.z -= fuzWidth;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.9f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 4
	position = masterPosition;

	position.x -= fuzWidth;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.9f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 5
	position = masterPosition;

	position.z += fuzWidth;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.9f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 6
	position = masterPosition;

	position.z += fuzWidth * 2.0f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 7
	position = masterPosition;

	position.x += fuzWidth * 1.7f;
	position.z += fuzWidth * 1.7f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 8
	position = masterPosition;

	position.x += fuzWidth * 2.0f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 9
	position = masterPosition;

	position.x += fuzWidth * 1.7f;
	position.z -= fuzWidth * 1.7f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 10
	position = masterPosition;

	position.z -= fuzWidth * 2.0f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 11
	position = masterPosition;

	position.x -= fuzWidth * 1.7f;
	position.z -= fuzWidth * 1.7f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 12
	position = masterPosition;

	position.x -= fuzWidth * 2.0f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************

	//*********************************************
	// Fuz 13
	position = masterPosition;

	position.x -= fuzWidth * 1.7f;
	position.z += fuzWidth * 1.7f;
	vertex.pos = position;
	verts[0] = vertex;

	position.x = position.x + fuzWidth;
	vertex.pos = position;
	verts[1] = vertex;

	position.x = position.x - halfWidth;
	position.z = position.z - fuzWidth;
	vertex.pos = position;
	verts[2] = vertex;

	position.z = position.z + halfWidth;
	position.y = position.y + fuzHeight * 0.6f;
	vertex.pos = position;
	verts[3] = vertex;

	for (uint i = 0; i < 4; i++)
	{
		vertex.color = vertex.color + float4(0.01f, 0.01f, 0.01f, 0.0f);
		verts[i].pos = mul(verts[i].pos, mVP);
		output.Append(verts[i]);
	}
	output.Append(verts[0]);
	output.Append(verts[1]);

	// cut this strip before making the next pyamid
	output.RestartStrip();
	//*********************************************
}