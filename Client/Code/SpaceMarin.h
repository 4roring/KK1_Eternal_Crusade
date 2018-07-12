#pragma once

#include "GameObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CAnimController;
}

class CController;

class CSpaceMarin
	: public Engine::CGameObject
{
private:
	explicit CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSpaceMarin();

private:
	HRESULT Initialize(int ctrl_id);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CSpaceMarin* Create(LPDIRECT3DDEVICE9 ptr_device, int ctrl_id = 0);

private:
	HRESULT AddComponent(int ctrl_id);
	void Release();

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;

private:
	Engine::CAnimController* ptr_anim_ctrl_ = nullptr;

private:
	CController* ptr_ctrl_ = nullptr;
	int current_cell_index_ = 0;
};
