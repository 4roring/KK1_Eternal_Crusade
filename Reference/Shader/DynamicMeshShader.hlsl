#include "BaseShader.hlsl"

float4x3 matrix_palette[55];
int num_bone_influences = 2;

float g_alpha = 1.f;

vector g_light_diffuse;
vector g_light_ambient;
vector g_light_dir;

vector g_mtrl_ambient = (vector) 0.2f;

struct VS_IN
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
    vector weights : BLENDWEIGHT0;
    int4 bone_indices : BLENDINDICES0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VS_OUT
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
    float3 light_dir : TEXCOORD1;
    float3 view_dir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
    vector new_pos = vector(0.f, 0.f, 0.f, 1.f);
    float3 new_normal = float3(0.f, 0.f, 0.f);
    float last_weight = 0.f;
    int num = num_bone_influences - 1;
    In.normal = normalize(In.normal);
    
    for (int i = 0; i < num; ++i)
    {
        last_weight += In.weights[i];
        new_pos += In.weights[i] * vector(mul(In.position, matrix_palette[In.bone_indices[i]]), 1.f);
        new_normal += In.weights[i] * mul(vector(In.normal, 0.f), matrix_palette[In.bone_indices[i]]);
    }

    last_weight = 1.f - last_weight;
    new_pos += last_weight * vector(mul(In.position, matrix_palette[In.bone_indices[num]]), 1.f);
    new_pos.w = 1.f;
    new_normal += last_weight * mul(vector(In.normal, 0.f), matrix_palette[In.bone_indices[num]]);

    vector mat_transform;

    mat_transform = mul(mul(mul(new_pos, g_mat_world), g_mat_view), g_mat_projection);

    Out.position = mat_transform;
	Out.texture_uv = In.texture_uv;
    Out.light_dir = g_light_dir;
    Out.T = mul(In.tangent, (float3x3) g_mat_world);
    Out.B = mul(In.binormal, (float3x3) g_mat_world);
    new_normal = normalize(new_normal);
    Out.N = mul(new_normal, (float3x3) g_mat_world);

	return Out;
}

texture g_color_texture;
sampler color_sampler = sampler_state
{
    texture = g_color_texture;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};
texture g_normal_texture;
sampler normal_sampler = sampler_state
{
    texture = g_normal_texture;
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
    float2 texture_uv : TEXCOORD0;
    float3 light_dir : TEXCOORD1;
    float3 view_dir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

struct PS_OUT
{
	vector color : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	
    float3 tangent_normal = tex2D(normal_sampler, In.texture_uv).xyz;
    tangent_normal = normalize(tangent_normal * 2.f - 1.f);
    float3x3 TBN = float3x3(normalize(In.T), normalize(In.B), normalize(In.N));
    TBN = transpose(TBN);

    float3 world_normal = mul(TBN, tangent_normal);
    float3 light_dir = normalize(In.light_dir);
    float3 diffuse = saturate(dot(world_normal, -light_dir));

    vector color = tex2D(color_sampler, In.texture_uv);

    diffuse = g_light_diffuse.rgb * color.rgb * diffuse;
    
    float3 ambient = g_mtrl_ambient * color;

    Out.color = vector(ambient + diffuse, 1.f);

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