#pragma once

#include "GameObject.h"

namespace Engine
{
	class CDynamicMesh;
	class CAnimController;
}

class CSpaceMarin
	: public Engine::CGameObject
{
private:
	explicit CSpaceMarin(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSpaceMarin();

private:
	virtual HRESULT Initialize() override;

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;

public:
	static CSpaceMarin* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;

private:
	Engine::CAnimController* ptr_anim_ctrl_ = nullptr;

private:
	UINT anim_track_ = 0;
};
