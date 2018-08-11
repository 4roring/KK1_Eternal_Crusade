#pragma once

#include "Mesh.h"

BEGIN(Engine)

struct MeshInfo
{
	std::list<BoneMesh*> data;
	Matrix init_matrix;
};

class CLoader;
class CAnimController;

class ENGINE_DLL CDynamicMesh
	: public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 ptr_device);
	
public:
	virtual ~CDynamicMesh();

public:
	const D3DXMATRIX* FindFrameMatrix(const char* ptr_frame_name) const;
	BoneFrame* FindFrame(const char * ptr_frame_name) const;

public:
	virtual CResources* CloneComponent() override;
	CAnimController* CloneAnimController() const;

public:
	void FrameMove(float delta_time, CAnimController* ptr_anim_ctrl);
	void FrameMove(float delta_time, CAnimController* ptr_anim_ctrl, BoneFrame* ptr_find_frame);
	void FrameMove(float delta_time, CAnimController* ptr_anim_ctrl, BoneFrame* ptr_find_frame, const Matrix* ptr_matrix);

public:
	virtual void RenderMesh(LPD3DXEFFECT ptr_effect, uint32 pass_index);

public:
	virtual int Release();

private:
	HRESULT LoadMeshFromFile(const TCHAR* path, const TCHAR* file_name);

private:
	void UpdateFrameMatrix(BoneFrame* ptr_frame, const Matrix* ptr_parent_matrix);
	void UpdateFrameMatrix(BoneFrame* ptr_frame, BoneFrame* ptr_find_frame, const Matrix* ptr_parent_matrix, const Matrix* mat_rot);
	void SetUpBoneMatrixPointer(BoneFrame* ptr_frame);
	void FindMeshContainer(BoneFrame* ptr_frame, LPD3DXEFFECT ptr_effect);
	void RenderMeshContainer(BoneMesh* ptr_mesh_container, LPD3DXEFFECT ptr_effect, uint32 pass_index);

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 ptr_device
		, const TCHAR* path, const TCHAR* file_name);

private:
	LPD3DXFRAME ptr_root_bone_ = nullptr;
	CLoader* ptr_loader_ = nullptr;
	CAnimController* ptr_anim_ctrl_ = nullptr;

private:
	MeshInfo mesh_info_;
};

END