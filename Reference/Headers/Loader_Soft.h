#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CLoader_Soft
	: public ID3DXAllocateHierarchy
{
private:
	CLoader_Soft(LPDIRECT3DDEVICE9 ptr_device, const TCHAR* path);

public:
	virtual ~CLoader_Soft();

public:
	STDOVERRIDEMETHODIMP CreateFrame(THIS_ LPCSTR ptr_name, LPD3DXFRAME* pp_new_frame);
	STDOVERRIDEMETHODIMP CreateMeshContainer(THIS_ LPCSTR ptr_name
		, CONST D3DXMESHDATA* ptr_mesh_data
		, CONST D3DXMATERIAL* ptr_materials
		, CONST D3DXEFFECTINSTANCE* ptr_effect_instance
		, DWORD num_materials
		, CONST DWORD *ptr_adjacency
		, LPD3DXSKININFO ptr_skin_info
		, LPD3DXMESHCONTAINER* pp_new_mesh_containder);
	STDOVERRIDEMETHODIMP DestroyFrame(THIS_ LPD3DXFRAME ptr_frame_to_free);
	STDOVERRIDEMETHODIMP DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER ptr_mesh_container_to_free);

public:
	static CLoader_Soft* Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR* path);

private:
	void AllocateName(CHAR** pp_new_name, const CHAR* const ptr_name);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	const TCHAR* path_ = nullptr;
};

END