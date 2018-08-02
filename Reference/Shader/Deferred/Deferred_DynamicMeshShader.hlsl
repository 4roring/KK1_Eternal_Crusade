#include "../BaseShader.hlsl"

float4 set_color_r;
float4 set_color_g;
float4 set_color_b;

float4x3 matrix_palette[55];
int num_bone_influences = 2;

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

// DEFAULT
struct VS_IN_DEFAULT
{
    vector position : POSITION;
    float2 texture_uv : TEXCOORD0;
    vector weights : BLENDWEIGHT0;
    int4 bone_indices : BLENDINDICES0;
    float3 normal : NORMAL;
};

struct VS_OUT_DEFAULT
{
    vector position : POSITION;
    float2 texture_uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    vector proj_pos : TEXCOORD2;
};

VS_OUT_DEFAULT VS_DEFAULT(VS_IN_DEFAULT vs_in)
{
    VS_OUT_DEFAULT vs_out = (VS_OUT_DEFAULT) 0;
	
    vector new_pos = vector(0.f, 0.f, 0.f, 1.f);
    float3 new_normal = float3(0.f, 0.f, 0.f);
    float last_weight = 0.f;
    int num = num_bone_influences - 1;
    vs_in.normal = normalize(vs_in.normal);
    
    for (int i = 0; i < num; ++i)
    {
        last_weight += vs_in.weights[i];
        new_pos += vs_in.weights[i] * vector(mul(vs_in.position, matrix_palette[vs_in.bone_indices[i]]), 1.f);
        new_normal += vs_in.weights[i] * mul(vector(vs_in.normal, 0.f), matrix_palette[vs_in.bone_indices[i]]);
    }

    last_weight = 1.f - last_weight;
    new_pos += last_weight * vector(mul(vs_in.position, matrix_palette[vs_in.bone_indices[num]]), 1.f);
    new_pos.w = 1.f;
    new_normal += last_weight * mul(vector(vs_in.normal, 0.f), matrix_palette[vs_in.bone_indices[num]]);

    vs_out.position = mul(mul(mul(new_pos, g_mat_world), g_mat_view), g_mat_projection);
    vs_out.texture_uv = vs_in.texture_uv;
    new_normal = normalize(new_normal);
    vs_out.normal = mul(new_normal, (float3x3) g_mat_world);

    return vs_out;
}

struct PS_IN_DEFAULT
{
    float2 texture_uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    vector proj_pos : TEXCOORD2;
};

struct PS_OUT_DEFAULT
{
    vector albedo : COLOR0;
    vector normal : COLOR1;
    vector depth : COLOR2;
};

PS_OUT_DEFAULT PS_DEFAULT(PS_IN_DEFAULT ps_in)
{
    PS_OUT_DEFAULT ps_out = (PS_OUT_DEFAULT) 0;
    ps_out.albedo = tex2D(color_sampler, ps_in.texture_uv);
    ps_out.normal = vector(ps_in.normal * 0.5f + 0.5f, 0.f);
    ps_out.depth = vector(ps_in.proj_pos.z / ps_in.proj_pos.w, ps_in.proj_pos.w * 0.001f, 0.f, 0.f);

    return ps_out;
}

// MAIN
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
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
    vector proj_pos : TEXCOORD6;
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

    Out.position = mul(mul(mul(new_pos, g_mat_world), g_mat_view), g_mat_projection);
	Out.texture_uv = In.texture_uv;

    Out.T = mul(In.tangent, (float3x3) g_mat_world);
    Out.B = mul(In.binormal, (float3x3) g_mat_world);
    new_normal = normalize(new_normal);
    Out.N = mul(new_normal, (float3x3) g_mat_world);

	return Out;
}

struct PS_IN
{
    float2 texture_uv : TEXCOORD0;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
    vector proj_pos : TEXCOORD6;
};

struct PS_OUT
{
	vector albedo : COLOR0;
    vector normal : COLOR1;
    vector depth : COLOR2;
    vector specular : COLOR3;
};

PS_OUT PS_MAIN(PS_IN ps_in)
{
	PS_OUT ps_out = (PS_OUT)0;
	
    float3 tangent_normal = tex2D(normal_sampler, ps_in.texture_uv).xyz;
    tangent_normal = normalize(tangent_normal * 2.f - 1.f);
    float3x3 TBN = float3x3(normalize(ps_in.T), normalize(ps_in.B), normalize(ps_in.N));
    TBN = transpose(TBN);

    float3 world_normal = mul(TBN, tangent_normal);
 
    ps_out.albedo = tex2D(color_sampler, ps_in.texture_uv);
    ps_out.normal = vector(world_normal * 0.5f + 0.5f, 0.f);
    ps_out.depth = vector(ps_in.proj_pos.z / ps_in.proj_pos.w, ps_in.proj_pos.w * 0.001f, 0.f, 0.f);
    ps_out.specular = tex2D(specular_sampler, ps_in.texture_uv);

    return ps_out;
}

PS_OUT PS_COLOR_MAIN(PS_IN ps_in)
{
    PS_OUT ps_out = (PS_OUT) 0;
	
    float3 tangent_normal = tex2D(normal_sampler, ps_in.texture_uv).xyz;
    tangent_normal = normalize(tangent_normal * 2.f - 1.f);
    float3x3 TBN = float3x3(normalize(ps_in.T), normalize(ps_in.B), normalize(ps_in.N));
    TBN = transpose(TBN);

    float3 world_normal = mul(TBN, tangent_normal);
 
    vector albedo = tex2D(color_sampler, ps_in.texture_uv);
    albedo = albedo.r * set_color_r + albedo.g + set_color_g + albedo.b + set_color_b;

    ps_out.albedo = albedo;
    ps_out.normal = vector(world_normal * 0.5f + 0.5f, 0.f);
    ps_out.depth = vector(ps_in.proj_pos.z / ps_in.proj_pos.w, ps_in.proj_pos.w * 0.001f, 0.f, 0.f);
    ps_out.specular = tex2D(specular_sampler, ps_in.texture_uv);

    return ps_out;
}

technique Default_Technique
{
	pass Default
	{
		VertexShader = compile vs_3_0 VS_DEFAULT();
        PixelShader = compile ps_3_0 PS_DEFAULT();
    }

    pass Normal
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass Color
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_COLOR_MAIN();
    }
}