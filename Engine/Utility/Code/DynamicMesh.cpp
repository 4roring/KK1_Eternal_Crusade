#include "DynamicMesh.h"
#include "AnimController.h"
#include "Loader.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 ptr_device)
	: CMesh(ptr_device)
{
}

Engine::CDynamicMesh::~CDynamicMesh()
{
}

void Engine::CDynamicMesh::GetComputeBoundingBox(const Vector3 & min, const Vector3 & max) const
{
}

void Engine::CDynamicMesh::ComputeMeshMinMax(D3DXFRAME * ptr_frame, const Vector3 & min, const Vector3 & max) const
{
}

const D3DXMATRIX * Engine::CDynamicMesh::FindFrame(const char * ptr_frame_name) const
{
	BoneFrame* ptr_frame = static_cast<BoneFrame*>(D3DXFrameFind(ptr_root_bone_, ptr_frame_name));

	return &ptr_frame->combined_matrix;
}

Engine::CResources * Engine::CDynamicMesh::CloneComponent()
{
	++reference_count_;
	return this;
}

Engine::CAnimController * Engine::CDynamicMesh::CloneAnimController()  const
{
	return ptr_anim_ctrl_->CloneAnimController();
}

void Engine::CDynamicMesh::FrameMove(float delta_time, CAnimController* ptr_anim_ctrl)
{
	ptr_anim_ctrl->FrameMove(delta_time);

	Matrix mat_identity;
	D3DXMatrixIdentity(&mat_identity);

	Matrix mat_scale, mat_rot_y;
	D3DXMatrixScaling(&mat_scale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&mat_rot_y, D3DXToRadian(179.99999f));
	mat_identity = mat_scale * mat_rot_y;

	UpdateFrameMatrix((BoneFrame*)ptr_root_bone_, &mat_identity);
}

void Engine::CDynamicMesh::RenderMesh(LPD3DXEFFECT ptr_effect)
{
	ptr_effect->Begin(nullptr, 0);
	FindMeshContainer((BoneFrame*)ptr_root_bone_, ptr_effect);
	ptr_effect->End();
}

int Engine::CDynamicMesh::Release()
{
	if (--reference_count_ == 0)
	{
		ptr_loader_->DestroyFrame(ptr_root_bone_);
		Safe_Delete(ptr_loader_);
		Safe_Delete(ptr_anim_ctrl_);
		return 0;
	}

	return reference_count_;
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR * path, const TCHAR * file_name)
{
	HRESULT hr = E_FAIL;
	TCHAR full_path[MAX_PATH] = L"";

	lstrcpy(full_path, path);
	lstrcat(full_path, file_name);

	ptr_loader_ = CLoader::Create(ptr_device_, path);
	ptr_anim_ctrl_ = CAnimController::Create();

	hr = D3DXLoadMeshHierarchyFromX(full_path
		, D3DXMESH_MANAGED, ptr_device_
		, ptr_loader_
		, nullptr
		, &ptr_root_bone_
		, ptr_anim_ctrl_->GetAnimController());
	assert(!FAILED(hr) && full_path && " Load Failed");

	ptr_anim_ctrl_->SetMaxAnimSet();

	D3DXMATRIX mat_identity;
	D3DXMatrixIdentity(&mat_identity);

	UpdateFrameMatrix((BoneFrame*)ptr_root_bone_, &mat_identity);
	SetUpFrameMatrixPointer((BoneFrame*)ptr_root_bone_);
	return S_OK;
}

void Engine::CDynamicMesh::UpdateFrameMatrix(BoneFrame * ptr_frame, const Matrix * ptr_parent_matrix)
{
	if (nullptr != ptr_frame)
		ptr_frame->combined_matrix = ptr_frame->TransformationMatrix * (*ptr_parent_matrix);

	if (nullptr != ptr_frame->pFrameFirstChild)
		UpdateFrameMatrix((BoneFrame*)ptr_frame->pFrameFirstChild, &ptr_frame->combined_matrix);

	if (nullptr != ptr_frame->pFrameSibling)
		UpdateFrameMatrix((BoneFrame*)ptr_frame->pFrameSibling, ptr_parent_matrix);
}

void Engine::CDynamicMesh::SetUpFrameMatrixPointer(BoneFrame * ptr_frame)
{
	if (nullptr == ptr_frame)
		return;

	LPD3DXMESHCONTAINER ptr_mesh = ptr_frame->pMeshContainer;
	if (nullptr != ptr_mesh)
	{
		if (ptr_mesh->pSkinInfo == nullptr)
			return;

		DWORD num_frame = ptr_mesh->pSkinInfo->GetNumBones();
		((BoneMesh*)ptr_mesh)->pp_frame_combined_matrix = new Matrix*[num_frame];
		ZeroMemory(((BoneMesh*)ptr_mesh)->pp_frame_combined_matrix
			, sizeof(Matrix*) * num_frame);


		for (DWORD i = 0; i < num_frame; ++i)
		{
			((BoneMesh*)ptr_mesh)->pp_frame_combined_matrix[i] =
				&((BoneFrame*)D3DXFrameFind(ptr_root_bone_, ptr_mesh->pSkinInfo->GetBoneName(i)))->combined_matrix;
		}
	}

	if (nullptr != ptr_frame->pFrameFirstChild)
		SetUpFrameMatrixPointer((BoneFrame*)ptr_frame->pFrameFirstChild);

	if (nullptr != ptr_frame->pFrameSibling)
		SetUpFrameMatrixPointer((BoneFrame*)ptr_frame->pFrameSibling);
}

void Engine::CDynamicMesh::FindMeshContainer(BoneFrame * ptr_frame, LPD3DXEFFECT ptr_effect)
{
	if (nullptr == ptr_frame)
		return;

	if (nullptr != ptr_frame->pMeshContainer)
		RenderMeshContainer((BoneMesh*)ptr_frame->pMeshContainer, ptr_effect);

	if (nullptr != ptr_frame->pFrameFirstChild)
		FindMeshContainer((BoneFrame*)ptr_frame->pFrameFirstChild, ptr_effect);

	if (nullptr != ptr_frame->pFrameSibling)
		FindMeshContainer((BoneFrame*)ptr_frame->pFrameSibling, ptr_effect);
}

void Engine::CDynamicMesh::RenderMeshContainer(BoneMesh * ptr_mesh_container, LPD3DXEFFECT ptr_effect)
{
	//if (nullptr != ptr_mesh_container->pSkinInfo)
	//{
	//	DWORD num_bones = ptr_mesh_container->pSkinInfo->GetNumBones();

	//	if (nullptr == ptr_mesh_container->ptr_result_matrix)
	//		ptr_mesh_container->ptr_result_matrix = new Matrix[num_bones];

	//	for (DWORD i = 0; i < num_bones; ++i)
	//	{
	//		ptr_mesh_container->ptr_result_matrix[i] = ptr_mesh_container->ptr_frame_offset_matrix[i]
	//			* (*ptr_mesh_container->pp_frame_combined_matrix[i]);
	//	}

	//	BYTE* ptr_src;
	//	BYTE* ptr_dst;

	//	ptr_mesh_container->ptr_original_mesh->LockVertexBuffer(0, (void**)&ptr_src);
	//	ptr_mesh_container->MeshData.pMesh->LockVertexBuffer(0, (void**)&ptr_dst);
	//
	//	ptr_mesh_container->pSkinInfo->UpdateSkinnedMesh(ptr_mesh_container->ptr_result_matrix
	//		, nullptr, ptr_src, ptr_dst);

	//	ptr_mesh_container->ptr_original_mesh->UnlockVertexBuffer();
	//	ptr_mesh_container->MeshData.pMesh->UnlockVertexBuffer();

	//	for (DWORD i = 0; i < ptr_mesh_container->NumMaterials; ++i)
	//	{
	//		ptr_device_->SetMaterial(&ptr_mesh_container->pMaterials[i].MatD3D);
	//		if(nullptr == ptr_effect)
	//			ptr_device_->SetTexture(0, ptr_mesh_container->pp_texture[i]);
	//		else
	//		{
	//			ptr_effect->SetTexture("g_base_texture", ptr_mesh_container->pp_texture[i]);
	//			ptr_effect->CommitChanges();
	//		}
	//		ptr_mesh_container->MeshData.pMesh->DrawSubset(i);
	//	}
	//}

	if (nullptr != ptr_mesh_container->pSkinInfo)
	{
		LPD3DXBONECOMBINATION ptr_bone_comb =
			(LPD3DXBONECOMBINATION)(ptr_mesh_container->ptr_bone_combination_buf->GetBufferPointer());

		DWORD num_bones = ptr_mesh_container->pSkinInfo->GetNumBones();

		if (nullptr == ptr_mesh_container->ptr_result_matrix)
			ptr_mesh_container->ptr_result_matrix = new Matrix[num_bones];

		for (DWORD i = 0; i < num_bones; ++i)
		{
			ptr_mesh_container->ptr_result_matrix[i] = ptr_mesh_container->ptr_frame_offset_matrix[i]
				* (*ptr_mesh_container->pp_frame_combined_matrix[i]);
		}
		
		ptr_effect->SetMatrixArray("matrix_palette", ptr_mesh_container->ptr_result_matrix, num_bones);

		ptr_effect->BeginPass(0);
		for (DWORD i = 0; i < ptr_mesh_container->NumMaterials; ++i)
		{
			if(nullptr != ptr_mesh_container->pp_color_texture[i])
				ptr_effect->SetTexture("g_color_texture", ptr_mesh_container->pp_color_texture[i]);
			if (nullptr != ptr_mesh_container->pp_color_texture[i])
				ptr_effect->SetTexture("g_normal_texture", ptr_mesh_container->pp_normal_texture[i]);
			if (nullptr != ptr_mesh_container->pp_color_texture[i])
				ptr_effect->SetTexture("g_specualr_texture", ptr_mesh_container->pp_specular_texture[i]);

			ptr_effect->CommitChanges();

			ptr_mesh_container->MeshData.pMesh->DrawSubset(i);
		}
		ptr_effect->EndPass();
	}
}

Engine::CDynamicMesh * Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * path, const TCHAR * file_name)
{
	CDynamicMesh* ptr_mesh = new CDynamicMesh(ptr_device);
	if (FAILED(ptr_mesh->LoadMeshFromFile(path, file_name)))
	{
		Safe_Release_Delete(ptr_mesh);
		assert(!"Dynamic Mesh Create Failed");
	}
	ptr_mesh->AddReferenceCount();
	return ptr_mesh;
}
