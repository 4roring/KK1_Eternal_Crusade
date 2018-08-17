#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CWarBossSkill
	: public Engine::CGameObject
{
private:
	explicit CWarBossSkill(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CWarBossSkill();

private:
	HRESULT Initialize(const Vector3& position);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CWarBossSkill* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_stone_effect_ = nullptr;
	Engine::CParticleSystem* ptr_smoke_effect_ = nullptr;

private:
	float life_time = 0.f;
};