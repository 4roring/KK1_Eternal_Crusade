#pragma once

#include <memory>

class ResourceLoader
{
public:
	enum class ResourceType { StaticMesh, Texture, End };

private:
	ResourceLoader();
	
public:
	~ResourceLoader();

private:
	HRESULT LoadStaticMesh(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box);
	HRESULT LoadTexture(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box);

public:
	static ResourceLoader* Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box, ResourceType type);
};

typedef std::auto_ptr<ResourceLoader> APRESOURCELOADER;