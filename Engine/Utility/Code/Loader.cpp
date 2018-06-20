#include "Loader.h"

Engine::CLoader::CLoader(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * path)
	: ptr_device_(ptr_device), path_(path)
{
}

Engine::CLoader::~CLoader()
{
}

STDOVERRIDEMETHODIMP Engine::CLoader::CreateFrame(LPCSTR ptr_name, LPD3DXFRAME * pp_new_frame)
{
	DerivedFrame* ptr_new_frame = new DerivedFrame;
	ZeroMemory(ptr_new_frame, sizeof(DerivedFrame));

	if (nullptr != ptr_name)
		AllocateName(&ptr_new_frame->Name, ptr_name);

	D3DXMatrixIdentity(&ptr_new_frame->combined_matrix);
	ptr_new_frame->TransformationMatrix = ptr_new_frame->combined_matrix;

	*pp_new_frame = ptr_new_frame;

	return S_OK;
}

STDOVERRIDEMETHODIMP Engine::CLoader::CreateMeshContainer(LPCSTR ptr_name
	, CONST D3DXMESHDATA * ptr_mesh_data
	, CONST D3DXMATERIAL * ptr_materials
	, CONST D3DXEFFECTINSTANCE * ptr_effect_instance
	, DWORD num_materials, CONST DWORD * ptr_adjacency
	, LPD3DXSKININFO ptr_skin_info
	, LPD3DXMESHCONTAINER * pp_new_mesh_containder)
{
	LPD3DXMESH ptr_mesh = ptr_mesh_data->pMesh;
	if (ptr_mesh->GetFVF() == 0)
		return E_FAIL;

	HRESULT hr = E_FAIL;
	DerivedMeshContainer* ptr_mesh_container = new DerivedMeshContainer;
	ZeroMemory(ptr_mesh_container, sizeof(DerivedMeshContainer));

	if (nullptr != ptr_name)
		AllocateName(&ptr_mesh_container->Name, ptr_name);

	DWORD num_faces = ptr_mesh->GetNumFaces();
	ptr_mesh_container->pAdjacency = new DWORD[num_faces * 3];
	memcpy(ptr_mesh_container->pAdjacency, ptr_adjacency, sizeof(DWORD) * num_faces * 3);

	//// Normal Data
	//if (!(ptr_mesh->GetFVF() & D3DFVF_NORMAL))
	//{
	//	hr = ptr_mesh->CloneMeshFVF(ptr_mesh->GetOptions(), ptr_mesh->GetFVF() | D3DFVF_NORMAL
	//		, ptr_device_, &ptr_mesh_container->MeshData.pMesh);
	//	assert(!FAILED(hr) && "CloneMeshFVF Failed");

	//	D3DXComputeNormals(ptr_mesh_container->MeshData.pMesh, ptr_mesh_container->pAdjacency);
	//}
	//else
	//{
	//	hr = ptr_mesh->CloneMeshFVF(ptr_mesh->GetOptions(), ptr_mesh->GetFVF() | D3DFVF_NORMAL
	//		, ptr_device_, &ptr_mesh_container->MeshData.pMesh);
	//	assert(!FAILED(hr) && "CloneMeshFVF Failed");

	//	ptr_mesh->AddRef();
	//}

	// Subset Data
	ptr_mesh_container->NumMaterials = max(num_materials, 1);
	ptr_mesh_container->pMaterials = new D3DXMATERIAL[ptr_mesh_container->NumMaterials];
	ptr_mesh_container->pp_texture = new LPDIRECT3DTEXTURE9[ptr_mesh_container->NumMaterials];
	ZeroMemory(ptr_mesh_container->pMaterials, sizeof(D3DXMATERIAL) * ptr_mesh_container->NumMaterials);
	ZeroMemory(ptr_mesh_container->pp_texture, sizeof(LPDIRECT3DTEXTURE9) * ptr_mesh_container->NumMaterials);

	if (num_materials > 0)
	{
		memcpy(ptr_mesh_container->pMaterials, ptr_materials, sizeof(D3DXMATERIAL) * ptr_mesh_container->NumMaterials);

		for (DWORD i = 0; i < num_materials; ++i)
		{
			if (nullptr == ptr_mesh_container->pMaterials[i].pTextureFilename)
				continue;

			TCHAR full_path[MAX_PATH] = TEXT("");
			TCHAR file_name[128] = TEXT("");

			lstrcpy(full_path, path_);
			MultiByteToWideChar(CP_ACP, 0, ptr_mesh_container->pMaterials[i].pTextureFilename
				, strlen(ptr_mesh_container->pMaterials[i].pTextureFilename)
				, file_name, 128);
			lstrcat(full_path, file_name);

			hr = D3DXCreateTextureFromFileW(ptr_device_, full_path, &ptr_mesh_container->pp_texture[i]);
			assert(!FAILED(hr) && "mesh_container Texture Create Failed");
		}
	}
	else
	{
		ZeroMemory(&ptr_mesh_container->pMaterials[0].MatD3D, sizeof(D3DMATERIAL9));
		ptr_mesh_container->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		ptr_mesh_container->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		ptr_mesh_container->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		ptr_mesh_container->pMaterials[0].MatD3D.Diffuse.a = 1.f;
		ptr_mesh_container->pMaterials[0].pTextureFilename = nullptr;
	}

	// Skin Info (Software Skinning)
	//if (nullptr != ptr_skin_info)
	//{
	//	ptr_mesh_container->pSkinInfo = ptr_skin_info;
	//	ptr_skin_info->AddRef();

	//	ptr_mesh_container->ptr_original_mesh = ptr_mesh;

	//	DWORD num_bones = ptr_skin_info->GetNumBones();
	//	ptr_mesh_container->ptr_frame_offset_matrix = new Matrix[num_bones];
	//	ZeroMemory(ptr_mesh_container->ptr_frame_offset_matrix, sizeof(Matrix) * num_bones);

	//	for (DWORD i = 0; i < num_bones; ++i)
	//		ptr_mesh_container->ptr_frame_offset_matrix[i] = *(ptr_skin_info->GetBoneOffsetMatrix(i));
	//}
	//else
	//{
	//	ptr_mesh_container->pSkinInfo = nullptr;
	//	ptr_mesh_container->ptr_frame_offset_matrix = nullptr;
	//	ptr_mesh_container->ptr_original_mesh = nullptr;
	//}

	// Skin Info (Hardware Skinning)
	if (nullptr != ptr_skin_info)
	{
		ptr_mesh_container->pSkinInfo = ptr_skin_info;
		ptr_skin_info->AddRef();

		DWORD max_vertex_in_fluences = 0;
		DWORD num_bone_combo_entries = 0;

		ptr_skin_info->ConvertToIndexedBlendedMesh(ptr_mesh_data->pMesh
			, D3DXMESH_MANAGED | D3DXMESH_WRITEONLY
			, 30
			, nullptr, nullptr, nullptr, nullptr
			, &max_vertex_in_fluences
			, &num_bone_combo_entries
			, &ptr_mesh_container->ptr_bone_combination_buf
			, &ptr_mesh_container->MeshData.pMesh);

		// Normal Data
		if (!(ptr_mesh->GetFVF() & D3DFVF_NORMAL))
			D3DXComputeNormals(ptr_mesh_container->MeshData.pMesh, ptr_mesh_container->pAdjacency);

		DWORD num_bones = ptr_skin_info->GetNumBones();
		ptr_mesh_container->ptr_frame_offset_matrix = new Matrix[num_bones];
		ZeroMemory(ptr_mesh_container->ptr_frame_offset_matrix, sizeof(Matrix) * num_bones);

		for (DWORD i = 0; i < num_bones; ++i)
			ptr_mesh_container->ptr_frame_offset_matrix[i] = *(ptr_skin_info->GetBoneOffsetMatrix(i));
	}

	*pp_new_mesh_containder = ptr_mesh_container;
	return S_OK;
}

STDOVERRIDEMETHODIMP Engine::CLoader::DestroyFrame(LPD3DXFRAME ptr_frame_to_free)
{
	if (nullptr == ptr_frame_to_free)
		return E_FAIL;

	if (nullptr != ptr_frame_to_free->pMeshContainer)
		DestroyMeshContainer(ptr_frame_to_free->pMeshContainer);

	if (nullptr != ptr_frame_to_free->pFrameFirstChild)
		DestroyFrame(ptr_frame_to_free->pFrameFirstChild);

	if (nullptr != ptr_frame_to_free->pFrameSibling)
		DestroyFrame(ptr_frame_to_free->pFrameSibling);

	Safe_Delete_Array(ptr_frame_to_free->Name);
	Safe_Delete(ptr_frame_to_free);
	return S_OK;
}

STDOVERRIDEMETHODIMP Engine::CLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER ptr_mesh_container_to_free)
{
	DerivedMeshContainer* ptr_mesh_container = static_cast<DerivedMeshContainer*>(ptr_mesh_container_to_free);
	for (DWORD i = 0; i < ptr_mesh_container_to_free->NumMaterials; ++i)
		Safe_Release(ptr_mesh_container->pp_texture[i]);

	Safe_Delete_Array(ptr_mesh_container->pp_texture);
	Safe_Delete_Array(ptr_mesh_container->pMaterials);
	Safe_Delete_Array(ptr_mesh_container->Name);
	Safe_Delete_Array(ptr_mesh_container->ptr_frame_offset_matrix);
	Safe_Delete_Array(ptr_mesh_container->pp_frame_combined_matrix);
	Safe_Delete_Array(ptr_mesh_container->ptr_result_matrix);
	Safe_Delete_Array(ptr_mesh_container->pAdjacency);

	Safe_Release(ptr_mesh_container->ptr_original_mesh);
	Safe_Release(ptr_mesh_container->MeshData.pMesh);
	Safe_Release(ptr_mesh_container->pSkinInfo);

	Safe_Delete(ptr_mesh_container);
	return S_OK;
}

Engine::CLoader * Engine::CLoader::Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * path)
{
	CLoader* ptr_loader = new CLoader(ptr_device, path);
	assert(nullptr != ptr_loader && "Loader Create Failed");

	return ptr_loader;
}

void Engine::CLoader::AllocateName(CHAR ** pp_new_name, const CHAR * const ptr_name)
{
	int length = strlen(ptr_name);

	*pp_new_name = new CHAR[length + 1];
	strcpy_s(*pp_new_name, length + 1, ptr_name);
}
