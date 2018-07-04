#pragma once


class ResourceLoader
{
private:
	ResourceLoader();
	
public:
	~ResourceLoader();

private:
	HRESULT LoadResource(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box);

public:
	static ResourceLoader* Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box);
};