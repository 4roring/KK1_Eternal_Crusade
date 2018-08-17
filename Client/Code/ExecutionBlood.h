#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CExecutionBlood
	: public Engine::CGameObject
{
private:
	explicit CExecutionBlood(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CExecutionBlood();

private:
	HRESULT Initialize(const Vector3& position);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CExecutionBlood* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;

private:
	float life_time = 0.f;
};