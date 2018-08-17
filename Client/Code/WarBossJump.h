#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CWarBossJump
	: public Engine::CGameObject
{
private:
	explicit CWarBossJump(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CWarBossJump();

private:
	HRESULT Initialize(Vector3* ptr_parent);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CWarBossJump* Create(LPDIRECT3DDEVICE9 ptr_device, Vector3* ptr_parent);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	Vector3* ptr_parent_ = nullptr;
};