#include "BaseShader.hlsl"

float g_max_u;
float g_max_v;
int g_frame_num;
int g_scene_num;
float g_alpha = 1.f;
float g_detail = 1.f;

texture g_color_texture;
sampler color_sampler = sampler_state
{
    texture = g_color_texture;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
    MIPFILTER = LINEAR;
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
    VS_OUT Out = (VS_OUT) 0;
	
    Matrix mat_transform;

    mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
    Out.texture_uv = In.texture_uv;

    return Out;
}

VS_OUT VS_UV(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	Matrix mat_transform;

	mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

	Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
    Out.texture_uv.x = float(In.texture_uv.x + g_frame_num) / g_max_u;
    Out.texture_uv.y = float(In.texture_uv.y + g_scene_num) / g_max_v;

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
    Out.color = tex2D(color_sampler, In.texture_uv);

	return Out;
}

PS_OUT PS_LINE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    float2 tex_uv = In.texture_uv;
    tex_uv.x *= g_detail;
    vector color = tex2D(color_sampler, tex_uv);
    Out.color = color;
    Out.color.a = color.r * g_alpha;

    return Out;
}

struct VS_IN_COLOR
{
    vector position : POSITION;
    float2 color : COLOR;
};

struct VS_OUT_COLOR
{
    vector position : POSITION;
    float2 color : COLOR;
};

VS_OUT_COLOR VS_COLOR(VS_IN_COLOR In)
{
    VS_OUT_COLOR Out = (VS_OUT) 0;
	
    Matrix mat_transform;

    mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
    Out.color = In.color;

    return Out;
}

vector PS_COLOR(vector color : COLOR) : COLOR
{
    return color;
}


technique Default_Technique
{
    pass DefaultEffect
    {
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;
        ZWriteEnable = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

	pass UV_Effect
	{
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;
        ZWriteEnable = FALSE;

        VertexShader = compile vs_3_0 VS_UV();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

    pass Line_Effect
    {
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;
        ZWriteEnable = FALSE;
    
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_LINE();
    }

    pass AlphaTest
    {
        ALPHATESTENABLE = TRUE;
        ALPHAFUNC = Greater;
        ALPHAREF = 0x0f;

        CULLMODE = NONE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass VertexColor
    {
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;

        VertexShader = compile vs_3_0 VS_COLOR();
        PixelShader = compile ps_3_0 PS_COLOR();
    }
}