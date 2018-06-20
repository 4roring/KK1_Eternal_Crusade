#pragma once

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CStaticMesh();

public:
	virtual CResources* CloneComponent() override;
	
public:
	virtual void GetComputeBoundingBox(const Vector3 & min, const Vector3 & max) const override;

public:
	virtual void RenderMesh(LPD3DXEFFECT ptr_effect) override;

public:
	virtual int Release() override;

private:
	HRESULT LoadMeshFromFile(const TCHAR* path, const TCHAR* file_name);

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR* path, const TCHAR* file_name);

private:
	bool CheckAlpha(int subset_index, const char* find_tag);

private:
	LPDIRECT3DTEXTURE9* pp_texture_ = nullptr;
	D3DMATERIAL9* ptr_material_ = nullptr;

private:
	LPD3DXMESH ptr_mesh_ = nullptr;
	LPD3DXBUFFER ptr_subset_buffer_ = nullptr;
	LPD3DXMATERIAL ptr_subset_ = nullptr;
	DWORD subset_count_ = 0;

private:
	DWORD reference_func_ = 0;
};

END