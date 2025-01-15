
struct VS_Input
{
	float4 Position : POSITION0;
	float4 Normal   : NORMAL0;
	float4 Color    : COLOR0;
	float4 Texcoord : TEXCOORD0;
};

struct VS_to_PS
{
	float4 Position : SV_Position;
	float4 Normal   : NORMAL0;
	float4 Color    : COLOR0;
	float4 Texcoord : TEXCOORD0;
};

struct PS_Output
{
	float4 Color : SV_Target0;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	
	float SpecularIntensity;
	int   TextureSamplingDisable;
	
	float2 Padding;
};
