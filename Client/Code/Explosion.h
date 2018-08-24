#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CExplosion
	: public Engine::CGameObject
{
private:
	explicit CExplosion(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CExplosion();

private:
	HRESULT Initialize(const Vector3& position, int effect_num);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CExplosion* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position, int effect_num);

private:
	HRESULT AddComponent(int effect_num);
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;

private:
	float life_time = 0.f;
};