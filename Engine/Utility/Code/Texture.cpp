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
	if (nullptr == texture_.vec_texture[index])
		return nullptr;

	return texture_.vec_texture[index];
}

Engine::CComponent * Engine::CTexture::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT Engine::CTexture::LoadTexture(TEXTURETYPE texture_type, const std::wstring & file_path, int count)
{
	IDirect3DBaseTexture9* ptr_texture = NULL;

	texture_.vec_texture.reserve(count);

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

		texture_.vec_texture.emplace_back(ptr_texture);
	}

	container_size = texture_.vec_texture.size();
	return S_OK;
}

HRESULT Engine::CTexture::LoadStaticMeshTexture(const std::wstring & file_path)
{
	IDirect3DBaseTexture9* ptr_texture = nullptr;

	HRESULT hr = E_FAIL;
	hr = D3DXCreateTextureFromFileW(ptr_device_, (file_path + TEXT("_BC.tga")).c_str(), (LPDIRECT3DTEXTURE9*)&ptr_texture);
	texture_.vec_texture.emplace_back(ptr_texture);

	hr = D3DXCreateTextureFromFileW(ptr_device_, (file_path + TEXT("_N.tga")).c_str(), (LPDIRECT3DTEXTURE9*)&ptr_texture);
	if (FAILED(hr)) ptr_texture = nullptr;
	texture_.vec_texture.emplace_back(ptr_texture);

	hr = D3DXCreateTextureFromFileW(ptr_device_, (file_path + TEXT("_R.tga")).c_str(), (LPDIRECT3DTEXTURE9*)&ptr_texture);
	if (FAILED(hr)) ptr_texture = nullptr;
	texture_.vec_texture.emplace_back(ptr_texture);

	container_size = texture_.vec_texture.size();
	return S_OK;
}

int Engine::CTexture::Release()
{
	if (--reference_count_ == 0)
	{
		for (auto& texture : texture_.vec_texture)
			Safe_Release(texture);

		texture_.vec_texture.clear();
		return 0;
	}
	return reference_count_;
}

Engine::CTexture * Engine::CTexture::Create(LPDIRECT3DDEVICE9 ptr_device
	, TEXTURETYPE texture_type, const std::wstring & file_path, int count)
{
	CTexture* ptr_texture = new CTexture(ptr_device);

	if (texture_type == TEXTURETYPE::STATIC_MESH)
	{
		if (FAILED(ptr_texture->LoadStaticMeshTexture(file_path)))
			Safe_Delete(ptr_texture);
	}
	else
	{
		if (FAILED(ptr_texture->LoadTexture(texture_type, file_path, count)))
			Safe_Delete(ptr_texture);
	}

	ptr_texture->AddReferenceCount();
	return ptr_texture;
}
