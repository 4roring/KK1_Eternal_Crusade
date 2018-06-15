#include "BaseShader.hlsl"

float g_alpha = 1.f;
texture g_base_texture;

sampler base_sampler = sampler_state
{
	texture = g_base_texture;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_IN
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
};

struct VS_OUT
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
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
	float2 texture_uv : TEXCOORD0;
};

struct PS_OUT
{
	vector color : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	
    vector color = tex2D(base_sampler, In.texture_uv);
    color = color.r * vector(0.28f, 0.61f, 0.05f, 1.f)
     + color.g * vector(0.25f, 0.25f, 0.25f, 1.f) + color.b * vector(0.3f, 0.3f, 0.3f, 1.f);

    Out.color = color;

	return Out;
}

technique Default_Technique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}