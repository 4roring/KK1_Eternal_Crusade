#include "BaseShader.hlsl"

texture g_color_texture;
sampler color_sampler = sampler_state
{
    texture = g_color_texture;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_IN
{
	vector position : POSITION;
	float3 texture_uv : TEXCOORD0;
};

struct VS_OUT
{
	vector position : POSITION;
	float3 texture_uv : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	Matrix mat_transform;

	mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

	Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
	Out.texture_uv = In.texture_uv;

	return Out;
}

struct PS_IN
{
	float3 texture_uv : TEXCOORD0;
};

struct PS_OUT
{
	vector color : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
    Out.color = texCUBE(color_sampler, In.texture_uv);

	return Out;
}

technique Default_Technique
{
	pass Default
	{
        ZWriteEnable = false;
        ZEnable = false;
        CULLMODE = NONE;

        VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}