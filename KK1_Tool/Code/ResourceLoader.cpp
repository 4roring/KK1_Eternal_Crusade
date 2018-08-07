#include "stdafx.h"
#include "ResourceLoader.h"
#include <filesystem>

#include "StaticMesh.h"

namespace FILESYSTEM = std::experimental::filesystem;

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

HRESULT ResourceLoader::LoadResource(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box)
{
	FILESYSTEM::path find_file_path(path);

	if (false == FILESYSTEM::is_directory(find_file_path))
		return E_FAIL;

	for (const auto& directory : FILESYSTEM::directory_iterator(find_file_path))
	{
		if (FILESYSTEM::is_directory(directory.status()))
			LoadResource(ptr_device, directory.path(), list_box);

		if (FILESYSTEM::is_regular_file(directory.status()) && directory.path().extension() == TEXT(".X"))
		{
			std::wstring file_path = directory.path().c_str();
			const std::wstring file_name = directory.path().filename().c_str();
			const std::wstring file_key = directory.path().stem().c_str();
			auto iter = file_path.rfind(TEXT("\\"));
			file_path = file_path.substr(0, ++iter);

			Engine::CStaticMesh* ptr_mesh = Engine::CStaticMesh::Create(ptr_device
				, file_path.c_str(), file_name.c_str(), 0);

			Engine::GameManager()->Add_Prototype(0, file_key, ptr_mesh);
			list_box.AddString(file_key.c_str());
		}
	}
	find_file_path.clear();

	return S_OK;
}

ResourceLoader * ResourceLoader::Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& path, CListBox& list_box)
{
	ResourceLoader* ptr_loader = new ResourceLoader;
	if (FAILED(ptr_loader->LoadResource(ptr_device, path, list_box)))
	{
		Engine::Safe_Delete(ptr_loader);
		assert(!"Tool Resource Loader Create Failed");
	}

	return ptr_loader;
}
