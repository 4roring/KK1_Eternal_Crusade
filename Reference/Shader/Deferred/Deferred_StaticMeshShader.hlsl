#include "../BaseShader.hlsl"

float4 set_color_r;
float4 set_color_g;
float4 set_color_b;

vector g_camera_position;

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
	
    Matrix mat_transform;

    mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    vector world_pos = mul(vector(vs_in.position.xyz, 1.f), mat_transform);

    vs_out.position = world_pos;
    vs_out.texture_uv = vs_in.texture_uv;
    vs_out.normal = mul(vs_in.normal, (float3x3) g_mat_world);
    vs_out.proj_pos = vs_out.position;

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

struct VS_IN
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct VS_OUT
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
    float3 T : TEXCOORD1;
    float3 B : TEXCOORD2;
    float3 N : TEXCOORD3;
    vector proj_pos : TEXCOORD4;
};

VS_OUT VS_MAIN(VS_IN vs_in)
{
	VS_OUT vs_out = (VS_OUT)0;
	
	Matrix mat_transform;

	mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    vector world_pos = mul(vector(vs_in.position.xyz, 1.f), mat_transform);

    vs_out.position = world_pos;
    vs_out.texture_uv = vs_in.texture_uv;
    
    vs_out.T = mul(vs_in.tangent, (float3x3) g_mat_world);
    vs_out.B = mul(vs_in.binormal, (float3x3) g_mat_world);
    vs_out.N = mul(vs_in.normal, (float3x3) g_mat_world);
    vs_out.proj_pos = vs_out.position;

    return vs_out;
}

struct PS_IN
{
	float2 texture_uv : TEXCOORD0;
    float3 T : TEXCOORD1;
    float3 B : TEXCOORD2;
    float3 N : TEXCOORD3;
    vector proj_pos : TEXCOORD4;
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
    ps_out.albedo.a = 1.f;
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
    ps_out.albedo.a = 1.f;
    ps_out.normal = vector(world_normal * 0.5f + 0.5f, 0.f);
    ps_out.depth = vector(ps_in.proj_pos.z / ps_in.proj_pos.w, ps_in.proj_pos.w * 0.001f, 0.f, 0.f);
    ps_out.specular = tex2D(specular_sampler, ps_in.texture_uv);

    return ps_out;
}

technique Default_Technique
{
    pass SetBumpMap
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }

    pass SetColor
    {
        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_COLOR_MAIN();
    }

	pass Default
	{
        VertexShader = compile vs_3_0 VS_DEFAULT();
		PixelShader = compile ps_3_0 PS_DEFAULT();
    }
}