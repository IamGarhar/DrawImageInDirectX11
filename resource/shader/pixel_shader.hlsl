
#include "shader_header.hlsli"

cbuffer MaterialBuffer : register(b0) { Material g_Material; }

Texture2D g_Texture         : register(t0);
SamplerState g_SamplerState : register(s0);

// main func
PS_Output main(VS_to_PS input)
{
	PS_Output output;
	
	float4 color = input.Color;

	// texture sampling
	if (!g_Material.TextureSamplingDisable)
	{
		color *= g_Texture.Sample(g_SamplerState, input.Texcoord.xy);
		color.rgb *= g_Material.Diffuse.rgb;
	}

	output.Color = color;

	return output;
}