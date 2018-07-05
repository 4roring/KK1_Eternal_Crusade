#include "StaticMesh.h"
#include "GameManager.h"
#include "Texture.h"

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

void Engine::CStaticMesh::GetComputeBoundingSphere(const Vector3 & min, const Vector3 & max) const
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
	
		if(nullptr != pp_color_texture_[i])
			ptr_effect->SetTexture("g_base_texture", pp_color_texture_[i]);
		else
			ptr_effect->SetTexture("g_base_texture", NULL);

		if (nullptr != pp_normal_texture_[i])
			ptr_effect->SetTexture("g_normal_texture", pp_normal_texture_[i]);
		else
			ptr_effect->SetTexture("g_normal_texture", NULL);

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
		{
			Safe_Release(pp_color_texture_[i]);
			Safe_Release(pp_normal_texture_[i]);
			Safe_Release(pp_specular_texture_[i]);
		}
		Safe_Delete_Array(pp_color_texture_);
		Safe_Delete_Array(pp_normal_texture_);
		Safe_Delete_Array(pp_specular_texture_);
		Safe_Release(ptr_subset_buffer_);
		Safe_Release(ptr_mesh_);

		return 0;
	}

	return reference_count_;
}

bool Engine::CStaticMesh::RaycastToMesh(const Vector3 & ray_pos, const Vector3 & ray_dir, float* hit_dir)
{
	HRESULT hr = E_FAIL;
	BOOL is_hit = false;
	hr = D3DXIntersect(ptr_mesh_, &ray_pos, &ray_dir, &is_hit, nullptr, nullptr, nullptr, hit_dir, nullptr, nullptr);
	
	if (FALSE == is_hit) return false;
	return true;
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR * path, const TCHAR * file_name, int stage_index)
{
	HRESULT hr = E_FAIL;
	std::wstring full_path;
	full_path += path;
	full_path += file_name;

	hr = D3DXLoadMeshFromXW(full_path.c_str(), D3DXMESH_MANAGED, ptr_device_
		, nullptr, &ptr_subset_buffer_, nullptr, &subset_count_, &ptr_mesh_);
	assert(!FAILED(hr) && "Static Mesh Load Failed");

	ptr_subset_ = (D3DXMATERIAL*)ptr_subset_buffer_->GetBufferPointer();

	ptr_material_ = new D3DMATERIAL9[subset_count_];
	pp_color_texture_ = new LPDIRECT3DTEXTURE9[subset_count_];
	pp_normal_texture_ = new LPDIRECT3DTEXTURE9[subset_count_];
	pp_specular_texture_ = new LPDIRECT3DTEXTURE9[subset_count_];

	auto iter = full_path.rfind(TEXT("\\"));
	full_path = full_path.substr(0, iter);
	iter = full_path.rfind(TEXT("\\"));
	full_path = full_path.substr(0, iter);
	full_path += TEXT("\\Texture\\");

	for (DWORD i = 0; i < subset_count_; ++i)
	{
		TCHAR tex_name_buf[128] = TEXT("");

		ptr_material_[i] = ptr_subset_[i].MatD3D;

		MultiByteToWideChar(CP_ACP, 0, ptr_subset_[i].pTextureFilename
			, strlen(ptr_subset_[i].pTextureFilename)
			, tex_name_buf, 128);

		std::wstring tex_key = tex_name_buf;
		auto iter = tex_key.rfind(TEXT("_"));
		tex_key = tex_key.substr(0, iter);

		CTexture* ptr_texture = dynamic_cast<CTexture*>(CGameManager::GetInstance()->CloneComponent(stage_index, tex_key));
		if (nullptr == ptr_texture)
		{
			ptr_texture = CTexture::Create(ptr_device_, TEXTURETYPE::STATIC_MESH, full_path + tex_key, 0);
			CGameManager::GetInstance()->Add_Prototype(stage_index, tex_key, ptr_texture);
			ptr_texture->AddReferenceCount();
		}
		pp_color_texture_[i] = (LPDIRECT3DTEXTURE9)ptr_texture->GetTexture(0);
		if(nullptr != pp_color_texture_[i]) pp_color_texture_[i]->AddRef();
		pp_normal_texture_[i] = (LPDIRECT3DTEXTURE9)ptr_texture->GetTexture(1);
		if (nullptr != pp_normal_texture_[i]) pp_normal_texture_[i]->AddRef();
		pp_specular_texture_[i] = (LPDIRECT3DTEXTURE9)ptr_texture->GetTexture(2);
		if (nullptr != pp_specular_texture_[i]) pp_specular_texture_[i]->AddRef();

		if (nullptr != ptr_texture) Safe_Release(ptr_texture);
	}
	return S_OK;
}

Engine::CStaticMesh * Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * path, const TCHAR * file_name, int stage_index)
{
	CStaticMesh* ptr_mesh = new CStaticMesh(ptr_device);
	if (FAILED(ptr_mesh->LoadMeshFromFile(path, file_name, stage_index)))
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
