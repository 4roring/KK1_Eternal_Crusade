#include "BaseShader.hlsl"

Matrix matrix_palette[35];
int num_bone_influences = 4;


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
    vector weights : BLENDWEIGHT0;
    int4 bone_indices : BLENDINDICES0;
};

struct VS_OUT
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
    vector _pos = vector(0.f, 0.f, 0.f, 1.f);
    //float3 normal = float3(0.f, 0.f, 0.f);
    float last_weight = 0.f;
    int num = num_bone_influences - 1;
    
    for (int i = 0; i < num; ++i)
    {
        last_weight += In.weights[i];
        _pos += In.weights[i] * mul(In.position, matrix_palette[In.bone_indices[i]]);
    }

    last_weight = 1.f - last_weight;
    _pos += last_weight * mul(In.position, matrix_palette[In.bone_indices[i]]);
    _pos.w = 1.f;

    vector mat_transform;

    mat_transform = mul(mul(mul(_pos, g_mat_world), g_mat_view), g_mat_projection);

    Out.position = mat_transform;
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