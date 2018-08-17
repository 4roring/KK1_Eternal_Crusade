#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CMoveDust
	: public Engine::CGameObject
{
private:
	explicit CMoveDust(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CMoveDust();

private:
	HRESULT Initialize(const Vector3& position);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CMoveDust* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	Matrix spawn_matrix_;

private:
	float life_time = 0.f;
};