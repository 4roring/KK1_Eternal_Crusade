#include "StaticMesh.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 ptr_device)
	: CMesh(ptr_device)
{
}

Engine::CStaticMesh::~CStaticMesh()
{
}

Engine::CResources * Engine::CStaticMesh::CloneComponent()
{
	++reference_count_;
	return this;
}

void Engine::CStaticMesh::GetComputeBoundingBox(const Vector3 & min, const Vector3 & max) const
{
}

void Engine::CStaticMesh::RenderMesh(LPD3DXEFFECT ptr_effect)
{
	ptr_effect->Begin(nullptr, 0);
	for (DWORD i = 0; i < subset_count_; ++i)
	{
		bool is_alpha = CheckAlpha(i, "A");

		int pass_index = 0;
		if (true == is_alpha)
			pass_index = 1;
		else
			pass_index = 0;

		ptr_effect->BeginPass(pass_index);
		ptr_effect->SetTexture("g_base_texture", pp_texture_[i]);
		ptr_effect->SetVector("g_material_diffuse", (Vector4*)&ptr_material_[i].Diffuse);
		ptr_effect->SetVector("g_material_ambient", &Vector4(1.f, 1.f, 1.f, 1.f));
		ptr_effect->SetVector("g_material_specular", &Vector4(1.f, 1.f, 1.f, 1.f));
		ptr_effect->SetFloat("g_light_power", ptr_material_[i].Power);
		ptr_effect->CommitChanges();
		ptr_mesh_->DrawSubset(i);
		ptr_effect->EndPass();
	}
	ptr_effect->End();
}

int Engine::CStaticMesh::Release()
{
	if (--reference_count_ == 0)
	{
		Safe_Delete_Array(ptr_material_);
		for (DWORD i = 0; i < subset_count_; ++i)
			Safe_Release(pp_texture_[i]);
		Safe_Delete_Array(pp_texture_);
		Safe_Release(ptr_subset_buffer_);
		Safe_Release(ptr_mesh_);

		return 0;
	}

	return reference_count_;
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR * path, const TCHAR * file_name)
{
	HRESULT hr = E_FAIL;
	TCHAR full_path[MAX_PATH] = TEXT("");

	lstrcpy(full_path, path);
	lstrcat(full_path, file_name);

	hr = D3DXLoadMeshFromXW(full_path, D3DXMESH_MANAGED, ptr_device_
		, nullptr, &ptr_subset_buffer_, nullptr, &subset_count_, &ptr_mesh_);
	assert(!FAILED(hr) && "Static Mesh Load Failed");

	ptr_subset_ = (D3DXMATERIAL*)ptr_subset_buffer_->GetBufferPointer();

	ptr_material_ = new D3DMATERIAL9[subset_count_];
	pp_texture_ = new LPDIRECT3DTEXTURE9[subset_count_];

	for (DWORD i = 0; i < subset_count_; ++i)
	{
		TCHAR file_name[128] = TEXT("");

		ptr_material_[i] = ptr_subset_[i].MatD3D;
		lstrcpy(full_path, path);
		MultiByteToWideChar(CP_ACP, 0, ptr_subset_[i].pTextureFilename
			, strlen(ptr_subset_[i].pTextureFilename)
			, file_name, 128);
		lstrcat(full_path, file_name);

		hr = D3DXCreateTextureFromFileW(ptr_device_, full_path, &pp_texture_[i]);
		assert(!FAILED(hr) && "Static Mesh Texture Load Failed");
	}
	return S_OK;
}

Engine::CStaticMesh * Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * path, const TCHAR * file_name)
{
	CStaticMesh* ptr_mesh = new CStaticMesh(ptr_device);
	if (FAILED(ptr_mesh->LoadMeshFromFile(path, file_name)))
	{
		Safe_Release_Delete(ptr_mesh);
		assert(!"StaticMesh Create Failed");
	}
	ptr_mesh->AddReferenceCount();
	return ptr_mesh;
}

bool Engine::CStaticMesh::CheckAlpha(int subset_index, const char * find_tag)
{
	int length = strlen(ptr_subset_[subset_index].pTextureFilename);

	for (int i = 0; i < length; ++i)
	{
		if ('.' == ptr_subset_[subset_index].pTextureFilename[i])
		{
			if (*find_tag == ptr_subset_[subset_index].pTextureFilename[i - 1])
				return true;
		}
	}

	return false;
}
