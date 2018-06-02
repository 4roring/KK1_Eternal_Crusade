#include "Texture.h"

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 ptr_device)
	: CResources(ptr_device)
{
}

Engine::CTexture::~CTexture()
{
}

const LPDIRECT3DBASETEXTURE9 Engine::CTexture::GetTexture(int index)
{
	return vec_texture_[index];
}

Engine::CComponent * Engine::CTexture::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT Engine::CTexture::LoadTexture(TEXTURETYPE texture_type, const std::wstring & file_path, int count)
{
	IDirect3DBaseTexture9* ptr_texture = NULL;

	vec_texture_.reserve(count);

	TCHAR full_path[MAX_PATH] = TEXT("");

	for (int i = 0; i < count; ++i)
	{
		wsprintf(full_path, file_path.c_str(), i);

		HRESULT hr = E_FAIL;

		switch (texture_type)
		{
		case TEXTURETYPE::NORMAL:
			hr = D3DXCreateTextureFromFile(ptr_device_, full_path, (LPDIRECT3DTEXTURE9*)&ptr_texture);
			break;
		case TEXTURETYPE::CUBE:
			hr = D3DXCreateCubeTextureFromFile(ptr_device_, full_path, (LPDIRECT3DCUBETEXTURE9*)&ptr_texture);
			break;
		}
		assert(hr == S_OK && "Create Texture Failed");

		vec_texture_.emplace_back(ptr_texture);
	}

	container_size = vec_texture_.size();
	return S_OK;
}

int Engine::CTexture::Release()
{
	if (--reference_count_ == 0)
	{
		for (auto& texture : vec_texture_)
			Safe_Release(texture);

		vec_texture_.clear();
		return 0;
	}
	return reference_count_;
}

Engine::CTexture * Engine::CTexture::Create(LPDIRECT3DDEVICE9 ptr_device
	, TEXTURETYPE texture_type, const std::wstring & file_path, int count)
{
	CTexture* ptr_texture = new CTexture(ptr_device);
	if (FAILED(ptr_texture->LoadTexture(texture_type, file_path, count)))
		Safe_Delete(ptr_texture);

	ptr_texture->AddReferenceCount();
	return ptr_texture;
}
