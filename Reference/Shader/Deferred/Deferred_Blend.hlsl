#include "../BaseShader.hlsl"

texture g_color_texture;
sampler color_sampler = sampler_state
{
    texture = g_color_texture;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};
texture g_shade_texture; 
sampler shade_sampler = sampler_state
{
    texture = g_shade_texture;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};
texture g_specular_texture; 
sampler specular_sampler = sampler_state
{
    texture = g_specular_texture;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

struct PS_IN
{
    vector position : POSITION;
    float2 texture_uv : TEXCOORD0;
};

vector PS_BLEND(PS_IN ps_in) : COLOR
{
    vector color[3];

    color[0] = tex2D(color_sampler, ps_in.texture_uv);
    color[1] = tex2D(shade_sampler, ps_in.texture_uv);
    color[2] = tex2D(specular_sampler, ps_in.texture_uv);
    color[2].a = 0.f;

    return color[0] * color[1] + color[2];
}

technique Default_Technique
{
    pass Blend
    {
        ZWriteEnable = false;

        AlphaTestEnable = true;
        AlphaFunc = greater;
        AlphaRef = 0x00;

        VertexShader = NULL;
        PixelShader = compile ps_3_0 PS_BLEND();
    }
}