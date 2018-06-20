#include "BaseShader.hlsl"

vector g_camera_position;

vector g_light_diffuse;
vector g_light_ambient;
vector g_light_dir;

vector g_mtrl_diffuse = (vector)1.f;
vector g_mtrl_ambient = (vector)0.2f;

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
    float3 light_dir : TEXCOORD1;
    float3 view_dir : TEXCOORD2;
    float3 T : TEXCOORD3;
    float3 B : TEXCOORD4;
    float3 N : TEXCOORD5;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;
	
	Matrix mat_transform;

	mat_transform = mul(mul(g_mat_world, g_mat_view), g_mat_projection);

    vector world_pos = mul(vector(In.position.xyz, 1.f), mat_transform);

    Out.position = world_pos;
	Out.texture_uv = In.texture_uv;
    Out.light_dir = g_light_dir;
    
    float3 view_dir = normalize(world_pos - g_camera_position);
    Out.view_dir = view_dir;

    Out.T = mul(In.tangent, (float3x3) g_mat_world);
    Out.B = mul(In.binormal, (float3x3) g_mat_world);
    Out.N = mul(In.normal, (float3x3) g_mat_world);

	return Out;
}

texture g_base_texture;
sampler base_sampler = sampler_state
{
    texture = g_base_texture;
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

    vector albedo = tex2D(base_sampler, In.texture_uv);
    diffuse = g_light_diffuse.rgb * albedo.rgb * diffuse;

    float3 ambient = g_mtrl_ambient * albedo;

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