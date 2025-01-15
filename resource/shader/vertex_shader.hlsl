
#include "shader_header.hlsli"

cbuffer WorldBuffer      : register(b0) { matrix g_MatrixWorld; }
cbuffer ViewBuffer       : register(b1) { matrix g_MatrixView; }
cbuffer ProjectoinBuffer : register(b2) { matrix g_MatrixProjection; }

// main func
VS_to_PS main( VS_Input input )
{
	VS_to_PS output;

	// perspective projection transformation
	matrix mvp;
	mvp = mul(g_MatrixWorld, g_MatrixView);
	mvp = mul(mvp, g_MatrixProjection);
	output.Position = mul(input.Position, mvp);

	// normal
	output.Normal = normalize(mul(input.Normal, g_MatrixWorld));

	// color
	output.Color = input.Color;

	// texcoord
	output.Texcoord = input.Texcoord;

	return output;
}
