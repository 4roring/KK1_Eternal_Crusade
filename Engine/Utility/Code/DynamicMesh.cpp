#include "DynamicMesh.h"
#include "AnimController.h"
#include "Loader.h"
#include "Loader_Soft.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 ptr_device)
	: CMesh(ptr_device)
{
	D3DXMatrixIdentity(&mesh_info_.init_matrix);
}

Engine::CDynamicMesh::~CDynamicMesh()
{
}

const D3DXMATRIX * Engine::CDynamicMesh::FindFrameMatrix(const char * ptr_frame_name) const
{
	BoneFrame* ptr_frame = static_cast<BoneFrame*>(D3DXFrameFind(ptr_root_bone_, ptr_frame_name));

	return &ptr_frame->combined_matrix;
}

Engine::BoneFrame * Engine::CDynamicMesh::FindFrame(const char * ptr_frame_name) const
{
	return static_cast<BoneFrame*>(D3DXFrameFind(ptr_root_bone_, ptr_frame_name));
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

void Engine::CDynamicMesh::SetInitMatrix(const Matrix & mat_init)
{
	mesh_info_.init_matrix = mat_init;
}

void Engine::CDynamicMesh::FrameMove(float delta_time, CAnimController* ptr_anim_ctrl)
{
	ptr_anim_ctrl->FrameMove(delta_time);
	UpdateFrameMatrix((BoneFrame*)ptr_root_bone_, &mesh_info_.init_matrix);
}

// 상체 FrameMove
void Engine::CDynamicMesh::FrameMove(float delta_time, CAnimController * ptr_anim_ctrl, BoneFrame * ptr_find_frame)
{
	ptr_anim_ctrl->FrameMove(delta_time);
	UpdateFrameMatrix((BoneFrame*)ptr_find_frame->pFrameFirstChild, &ptr_find_frame->combined_matrix);
}

// 하체 FrameMove
void Engine::CDynamicMesh::FrameMove(float delta_time, CAnimController * ptr_anim_ctrl, BoneFrame * ptr_find_frame, const Matrix * ptr_matrix)
{
	ptr_anim_ctrl->FrameMove(delta_time);
	UpdateFrameMatrix((BoneFrame*)ptr_root_bone_, ptr_find_frame, &mesh_info_.init_matrix, ptr_matrix);
}

void Engine::CDynamicMesh::RenderMesh(LPD3DXEFFECT ptr_effect, uint32 pass_index)
{
	ptr_effect->Begin(nullptr, 0);
	//FindMeshContainer((BoneFrame*)ptr_root_bone_, ptr_effect);
	for (auto& bone_mesh : mesh_info_.data)
		RenderMeshContainer(bone_mesh, ptr_effect, pass_index);
	ptr_effect->End();
}

void Engine::CDynamicMesh::RenderMesh_Soft(LPD3DXEFFECT ptr_effect, uint32 pass_index)
{
	ptr_effect->Begin(nullptr, 0);
	for (auto& bone_mesh : mesh_info_.data)
		RenderMeshContainer_Soft(bone_mesh, ptr_effect, pass_index);
	ptr_effect->End();
}

int Engine::CDynamicMesh::Release()
{
	if (--reference_count_ == 0)
	{
		if (nullptr != ptr_loader_)
			ptr_loader_->DestroyFrame(ptr_root_bone_);
		if(nullptr != ptr_loader_soft_)
			ptr_loader_soft_->DestroyFrame(ptr_root_bone_);

		Safe_Delete(ptr_loader_);
		Safe_Delete(ptr_loader_soft_);
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

	Matrix mat_scale, mat_rot_y;
	D3DXMatrixScaling(&mat_scale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&mat_rot_y, D3DXToRadian(179.99999f));
	mesh_info_.init_matrix = mat_scale * mat_rot_y;

	UpdateFrameMatrix((BoneFrame*)ptr_root_bone_, &mesh_info_.init_matrix);
	SetUpBoneMatrixPointer((BoneFrame*)ptr_root_bone_);

	return S_OK;
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile_Soft(const TCHAR * path, const TCHAR * file_name)
{
	HRESULT hr = E_FAIL;
	TCHAR full_path[MAX_PATH] = L"";

	lstrcpy(full_path, path);
	lstrcat(full_path, file_name);

	ptr_loader_soft_ = CLoader_Soft::Create(ptr_device_, path);
	ptr_anim_ctrl_ = CAnimController::Create();

	hr = D3DXLoadMeshHierarchyFromX(full_path
		, D3DXMESH_MANAGED, ptr_device_
		, ptr_loader_soft_
		, nullptr
		, &ptr_root_bone_
		, ptr_anim_ctrl_->GetAnimController());
	assert(!FAILED(hr) && full_path && " Load Failed");

	ptr_anim_ctrl_->SetMaxAnimSet();

	Matrix mat_scale, mat_rot_y;
	D3DXMatrixScaling(&mat_scale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&mat_rot_y, D3DXToRadian(179.99999f));
	mesh_info_.init_matrix = mat_scale * mat_rot_y;

	UpdateFrameMatrix((BoneFrame*)ptr_root_bone_, &mesh_info_.init_matrix);
	SetUpBoneMatrixPointer((BoneFrame*)ptr_root_bone_);

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

// 하체 UpdateFrameMatrix
void Engine::CDynamicMesh::UpdateFrameMatrix(BoneFrame * ptr_frame, BoneFrame * ptr_find_frame, const Matrix * ptr_parent_matrix, const Matrix * mat_rot)
{
	if (nullptr != ptr_frame->pFrameSibling)
		UpdateFrameMatrix((BoneFrame*)ptr_frame->pFrameSibling, ptr_find_frame, ptr_parent_matrix, mat_rot);

	if (ptr_frame == ptr_find_frame && nullptr != mat_rot)
	{
		ptr_frame->combined_matrix = ptr_frame->TransformationMatrix * (*mat_rot) * (*ptr_parent_matrix);
		return;
	}
	else
		ptr_frame->combined_matrix = ptr_frame->TransformationMatrix * (*ptr_parent_matrix);

	if (nullptr != ptr_frame->pFrameFirstChild)
		UpdateFrameMatrix((BoneFrame*)ptr_frame->pFrameFirstChild, ptr_find_frame, &ptr_frame->combined_matrix, mat_rot);
}

void Engine::CDynamicMesh::SetUpBoneMatrixPointer(BoneFrame * ptr_frame)
{
	if (nullptr == ptr_frame)
		return;

	LPD3DXMESHCONTAINER ptr_mesh = ptr_frame->pMeshContainer;
	if (nullptr != ptr_mesh)
	{
		mesh_info_.data.emplace_back((BoneMesh*)ptr_mesh);

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
		SetUpBoneMatrixPointer((BoneFrame*)ptr_frame->pFrameFirstChild);

	if (nullptr != ptr_frame->pFrameSibling)
		SetUpBoneMatrixPointer((BoneFrame*)ptr_frame->pFrameSibling);
}

void Engine::CDynamicMesh::FindMeshContainer(BoneFrame * ptr_frame, LPD3DXEFFECT ptr_effect)
{
	//if (nullptr == ptr_frame)
	//	return;

	//if (nullptr != ptr_frame->pMeshContainer)
	//	RenderMeshContainer((BoneMesh*)ptr_frame->pMeshContainer, ptr_effect);

	//if (nullptr != ptr_frame->pFrameFirstChild)
	//	FindMeshContainer((BoneFrame*)ptr_frame->pFrameFirstChild, ptr_effect);

	//if (nullptr != ptr_frame->pFrameSibling)
	//	FindMeshContainer((BoneFrame*)ptr_frame->pFrameSibling, ptr_effect);
}

void Engine::CDynamicMesh::RenderMeshContainer(BoneMesh * ptr_bone_mesh, LPD3DXEFFECT ptr_effect, uint32 pass_index)
{
	if (nullptr != ptr_bone_mesh->pSkinInfo)
	{
		LPD3DXBONECOMBINATION ptr_bone_comb =
			(LPD3DXBONECOMBINATION)(ptr_bone_mesh->ptr_bone_combination_buf->GetBufferPointer());

		// 현재 메쉬에 영향을 미치는 뼈의 갯수
		DWORD num_bones = ptr_bone_mesh->pSkinInfo->GetNumBones();

		if (nullptr == ptr_bone_mesh->ptr_result_matrix)
			ptr_bone_mesh->ptr_result_matrix = new Matrix[num_bones];

		for (DWORD i = 0; i < num_bones; ++i)
		{
			ptr_bone_mesh->ptr_result_matrix[i] = ptr_bone_mesh->ptr_frame_offset_matrix[i]
				* (*ptr_bone_mesh->pp_frame_combined_matrix[i]);
		}
		
		ptr_effect->SetMatrixArray("matrix_palette", ptr_bone_mesh->ptr_result_matrix, num_bones);

		ptr_effect->BeginPass(pass_index);
		for (DWORD i = 0; i < ptr_bone_mesh->NumMaterials; ++i)
		{
			if(nullptr != ptr_bone_mesh->pp_color_texture[i])
				ptr_effect->SetTexture("g_color_texture", ptr_bone_mesh->pp_color_texture[i]);
			if (nullptr != ptr_bone_mesh->pp_color_texture[i])
				ptr_effect->SetTexture("g_normal_texture", ptr_bone_mesh->pp_normal_texture[i]);
			//else
			//	pass_index = 0;
				
			if (nullptr != ptr_bone_mesh->pp_color_texture[i])
				ptr_effect->SetTexture("g_specular_texture", ptr_bone_mesh->pp_specular_texture[i]);
			//else
			//	pass_index = 0;

			ptr_effect->CommitChanges();
			ptr_bone_mesh->MeshData.pMesh->DrawSubset(i);
		
		}
		ptr_effect->EndPass();
	}
}

void Engine::CDynamicMesh::RenderMeshContainer_Soft(BoneMesh * ptr_bone_mesh, LPD3DXEFFECT ptr_effect, uint32 pass_index)
{	
	// Software Skinning
	if (nullptr != ptr_bone_mesh->pSkinInfo)
	{
		DWORD num_bones = ptr_bone_mesh->pSkinInfo->GetNumBones();

		if (nullptr == ptr_bone_mesh->ptr_result_matrix)
			ptr_bone_mesh->ptr_result_matrix = new Matrix[num_bones];

		for (DWORD i = 0; i < num_bones; ++i)
		{
			ptr_bone_mesh->ptr_result_matrix[i] = ptr_bone_mesh->ptr_frame_offset_matrix[i]
				* (*ptr_bone_mesh->pp_frame_combined_matrix[i]);
		}

		BYTE* ptr_src;
		BYTE* ptr_dst;

		ptr_bone_mesh->ptr_original_mesh->LockVertexBuffer(0, (void**)&ptr_src);
		ptr_bone_mesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&ptr_dst);
	
		ptr_bone_mesh->pSkinInfo->UpdateSkinnedMesh(ptr_bone_mesh->ptr_result_matrix
			, nullptr, ptr_src, ptr_dst);

		ptr_bone_mesh->ptr_original_mesh->UnlockVertexBuffer();
		ptr_bone_mesh->MeshData.pMesh->UnlockVertexBuffer();

		ptr_effect->BeginPass(pass_index);
		for (DWORD i = 0; i < ptr_bone_mesh->NumMaterials; ++i)
		{
			if (nullptr != ptr_bone_mesh->pp_color_texture[i])
				ptr_effect->SetTexture("g_color_texture", ptr_bone_mesh->pp_color_texture[i]);
		
			ptr_effect->CommitChanges();
			ptr_bone_mesh->MeshData.pMesh->DrawSubset(i);
		}
		ptr_effect->EndPass();
	}
}

Engine::CDynamicMesh * Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR * path, const TCHAR * file_name)
{
	CDynamicMesh* ptr_mesh = new CDynamicMesh(ptr_device);

	HRESULT hr = E_FAIL;
	if (0 == lstrcmp(file_name, TEXT("Rioreus.X")))
		hr = ptr_mesh->LoadMeshFromFile_Soft(path, file_name);
	else
		hr = ptr_mesh->LoadMeshFromFile(path, file_name);

	if (FAILED(hr))
	{
		Safe_Release_Delete(ptr_mesh);
		assert(!"Dynamic Mesh Create Failed");
	}

	ptr_mesh->AddReferenceCount();
	return ptr_mesh;
}
