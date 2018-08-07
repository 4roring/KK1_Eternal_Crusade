#include "../BaseShader.hlsl"

vector g_light_dir;
vector g_light_diffuse;
vector g_light_ambient;
vector g_light_specular;
vector g_power = 15.f;

vector g_light_pos;
float g_light_range;

vector g_camera_position;

vector g_inv_mat_view;
vector g_inv_mat_proj;

texture g_normal_texture;
sampler normal_sampler = sampler_state
{
    texture = g_normal_texture;
};
texture g_depth_texture;
sampler depth_sampler = sampler_state
{
    texture = g_depth_texture;
};
texture g_specular_texture;
sampler specular_sampler = sampler_state
{
    texture = g_specular_texture;
};

struct PS_IN
{
    vector position : POSITION;
    float2 texture_uv : TEXCOORD0;
};

struct PS_OUT
{
    vector shade : COLOR0;
    vector specular : COLOR1;
};

PS_OUT PS_DIRECTIONAL(PS_IN ps_in)
{
    PS_OUT ps_out = (PS_OUT) 0;

    vector normal = tex2D(normal_sampler, ps_in.texture_uv);
    normal.xyz = normal.xyz * 2.f - 1.f;

    ps_out.shade.rgb = max(dot(normalize(g_light_dir) * -1.f, normal), 0.f);
    ps_out.shade.a = 1.f;
    ps_out.shade = g_light_diffuse * ps_out.shade + g_light_ambient;

    vector reflect_light = normalize(reflect(g_light_dir, normal));

    vector depth = tex2D(depth_sampler, ps_in.texture_uv);
    float view_z = depth.g * 1000.f;

    vector proj_pos = (vector) 0.f;
    proj_pos.x = (ps_in.texture_uv.x * 2.f - 1.f) * view_z;
    proj_pos.y = (ps_in.texture_uv.y * -2.f + 1.f) * view_z;
    proj_pos.z = depth.r * view_z;

    vector specular_map = tex2D(specular_sampler, ps_in.texture_uv);

    vector world_pos = mul(mul(proj_pos, g_inv_mat_proj), g_inv_mat_view);
    vector look = normalize(world_pos - g_camera_position);
    ps_out.specular = pow((max(dot(reflect_light * -1.f, look), 0.f)), g_power) * (g_light_specular * specular_map);

    return ps_out;
}

PS_OUT PS_POINT(PS_IN ps_in)
{
    PS_OUT ps_out = (PS_OUT) 0;

    vector normal = tex2D(normal_sampler, ps_in.texture_uv);
    normal.xyz = normal.xyz * 2.f - 1.f;

    ps_out.shade.rgb = saturate(max(dot(normalize(g_light_dir) * -1.f, normal), 0.f));
    ps_out.shade.a = 1.f;
    ps_out.shade = saturate(g_light_diffuse * ps_out.shade + g_light_ambient);

    vector reflect_light = normalize(reflect(g_light_dir, normal));

    vector depth = tex2D(depth_sampler, ps_in.texture_uv);
    float view_z = depth.g * 1000.f;

    vector proj_pos = (vector)0.f;
    proj_pos.x = (ps_in.texture_uv.x * 2.f - 1.f) * view_z;
    proj_pos.y = (ps_in.texture_uv.y * -2.f + 1.f) * view_z;
    proj_pos.z = depth.r * view_z;

    vector world_pos = mul(mul(proj_pos, g_inv_mat_proj), g_inv_mat_view);
    vector look = world_pos - g_camera_position;
    ps_out.specular = pow(max(dot(reflect_light * -1.f, look), 0.f), g_power);

    return ps_out;
}


technique Default_Technique
{
    pass Directional
    {
        ZWriteEnable = false;

        AlphaBlendEnable = true;
        SrcBlend = one;
        DestBlend = one;

        VertexShader = NULL;
        PixelShader = compile ps_3_0 PS_DIRECTIONAL();
    }
    pass Point
    {
        ZWriteEnable = false;

        AlphaBlendEnable = true;
        SrcBlend = one;
        DestBlend = one;

        VertexShader = NULL;
        PixelShader = compile ps_3_0 PS_POINT();
    }
}


//float3 diffuse = saturate(dot(world_normal, -light_dir));