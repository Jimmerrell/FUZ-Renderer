struct INPUT_VERTEX
{
	float2 coordinate : POSITION;
	float4 colorIN : COLOR;
	float2 texCoord : TEXCOORD;
	float gradient : TEXCOORD1;
};

struct OUTPUT_VERTEX
{
	float2 texCoord : TEXCOORD0;
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float gradient : TEXCOORD1;
};

OUTPUT_VERTEX main(INPUT_VERTEX fromVertexBuffer)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	sendToRasterizer.projectedCoordinate.w = 1;

	sendToRasterizer.projectedCoordinate.xy = fromVertexBuffer.coordinate.xy;

	sendToRasterizer.colorOut = fromVertexBuffer.colorIN;

	sendToRasterizer.texCoord.x = fromVertexBuffer.texCoord.x;
	sendToRasterizer.texCoord.y = fromVertexBuffer.texCoord.y;

	sendToRasterizer.gradient = fromVertexBuffer.gradient; 

	return sendToRasterizer;
}