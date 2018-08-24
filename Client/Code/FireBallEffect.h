#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CFireBallEffect
	: public Engine::CGameObject
{
private:
	explicit CFireBallEffect(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CFireBallEffect();

private:
	HRESULT Initialize(const Matrix* ptr_parent);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CFireBallEffect* Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix* ptr_parent);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	const Matrix* ptr_parent_ = nullptr;
};