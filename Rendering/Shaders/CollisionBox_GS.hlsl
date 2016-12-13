#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

struct GSOutput
{
	float4 color : COLOR;
	float4 pos : SV_POSITION;
};

[maxvertexcount(4)]
void main(line INPUT_VERTEX input[2], inout TriangleStream< GSOutput > output )
{

	GSOutput element;

	element.pos = input[0].pos;
	element.color = input[0].color;

	output.Append(element);

	float3 offset = float3(0.0f, 1.0f, 0.0f);
	element.pos = input[0].pos + float4(offset, 1.0f);

	output.Append(element);

	element.pos = input[1].pos;

	output.Append(element);

	offset = float3(0.0f, 1.0f, 0.0f);
	element.pos = input[1].pos + float4(offset, 1.0f);

	output.Append(element);

}