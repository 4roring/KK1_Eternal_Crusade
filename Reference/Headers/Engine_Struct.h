#pragma once

namespace Engine
{
	typedef struct tagFrame_Derived : public D3DXFRAME
	{
		Matrix combined_matrix;
	}DerivedFrame;

	typedef struct tagMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9* pp_texture;
		LPD3DXMESH ptr_original_mesh;
		Matrix* ptr_frame_offset_matrix;
		Matrix** pp_frame_combined_matrix;
		Matrix* ptr_result_matrix;
	}DerivedMeshContainer;

	typedef struct tagVertexColor
	{
		Vector3 position;
		DWORD color;
	} VertexColor;

	const DWORD VertexColor_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texture_uv;
	}VertexTex;

	const DWORD VertexTex_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagCubeTexture
	{
		Vector3 position;
		Vector3 texture_uv;
	}CubeTex;

	const DWORD CubeTex_FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagTrailTexture
	{
		Vector3 position;
		Vector2 texture_uv;
	} TrailTexture;

	typedef struct tagIndex16
	{
		WORD _1, _2, _3;
	}Index16;

	typedef struct tagIndex32
	{
		DWORD _1, _2, _3;
	}Index32;
}