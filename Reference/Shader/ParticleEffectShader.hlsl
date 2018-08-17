#include "BaseShader.hlsl"

struct VS_IN
{
	vector position : POSITION;
	vector color : COLOR;
};

struct VS_OUT
{
	vector position : POSITION;
    vector color : COLOR;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
	
    Matrix mat_transform;

    mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
    Out.color = In.color;

    return Out;
}

struct PS_IN
{
    vector color : COLOR0;
};

vector PS_MAIN(PS_IN In) : COLOR
{
    return In.color;
}

technique Default_Technique
{
    pass DefaultEffect
    {
        ALPHABLENDENABLE = TRUE;
        SRCBLEND = SRCALPHA;
        DESTBLEND = INVSRCALPHA;
        CULLMODE = NONE;

        POINTSPRITEENABLE = TRUE;
        POINTSCALEENABLE = TRUE;
        POINTSIZE = 3.f;
        POINTSIZE_MIN = 0.f;

        POINTSCALE_A = 0.f;
        POINTSCALE_B = 0.f;
        POINTSCALE_C = 1.f;
        
        ZWRITEENABLE = false;
        
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }
}