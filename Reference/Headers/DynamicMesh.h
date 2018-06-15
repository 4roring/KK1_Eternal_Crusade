#pragma once

#include "Mesh.h"

BEGIN(Engine)

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
	virtual void GetComputeBoundingBox(const Vector3& min, const Vector3& max) const;
	void ComputeMeshMinMax(D3DXFRAME* ptr_frame, const Vector3& min, const Vector3& max) const;
	const D3DXMATRIX* FindFrame(const char* ptr_frame_name) const;

public:
	virtual CResources* CloneComponent() override;
	CAnimController* CloneAnimController() const;
	CAnimController* GetAnimController() const;

public:
	void FrameMove(float delta_time, CAnimController* ptr_anim_ctrl);

public:
	virtual void RenderMesh(LPD3DXEFFECT ptr_effect);

public:
	virtual int Release();

private:
	HRESULT LoadMeshFromFile(const TCHAR* path, const TCHAR* file_name);

private:
	void UpdateFrameMatrix(DerivedFrame* ptr_frame, const Matrix* ptr_parent_matrix);
	void SetUpFrameMatrixPointer(DerivedFrame* ptr_frame);
	void FindMeshContainer(DerivedFrame* ptr_frame, LPD3DXEFFECT ptr_effect);
	void RenderMeshContainer(DerivedMeshContainer* ptr_mesh_container, LPD3DXEFFECT ptr_effect);

public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 ptr_device
		, const TCHAR* path, const TCHAR* file_name);

private:
	LPD3DXFRAME ptr_root_bone_ = nullptr;
	CLoader* ptr_loader_ = nullptr;
	CAnimController* ptr_anim_ctrl_ = nullptr;
};

END