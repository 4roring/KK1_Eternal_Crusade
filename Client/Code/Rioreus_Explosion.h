#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
class CCollider;
END

class CRioreus_Explosion
	: public Engine::CGameObject
{
private:
	explicit CRioreus_Explosion(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CRioreus_Explosion();

private:
	HRESULT Initialize(const Vector3& position);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CRioreus_Explosion* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	Engine::CCollider* ptr_collider_ = nullptr;

private:
	float life_time = 0.f;

#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
#endif // _DEBUG

};