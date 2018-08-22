#include "BaseShader.hlsl"

float g_alpha = 1.f;

float g_max_u = 1.f;
float g_max_v = 1.f;

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

VS_OUT VS_UV(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
    Matrix mat_transform;

    mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
    Out.texture_uv = float2(In.texture_uv.x * g_max_u, In.texture_uv.y * g_max_v);

    return Out;
}

struct PS_IN
{
	float2 texture_uv : TEXCOORD0;
};

vector PS_MAIN(PS_IN In) : COLOR
{
    return tex2D(color_sampler, In.texture_uv);
}

vector PS_ALPHA(PS_IN In) : COLOR
{
    vector color = tex2D(color_sampler, In.texture_uv);
    color.a *= g_alpha;
    return color;
}

technique Default_Technique
{
	pass Default
	{
		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;

        VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

    pass UV
    {
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;

        VertexShader = compile vs_3_0 VS_UV();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass Alpha
    {
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_ALPHA();
    }
}