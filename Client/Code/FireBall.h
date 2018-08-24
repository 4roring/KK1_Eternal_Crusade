#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CCollider;
END

class CFireBall
	: public Engine::CGameObject
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CFireBall();

private:
	HRESULT Initialize(const Vector3& position, const Vector3& move_dir);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position, const Vector3& move_dir);

private:
	HRESULT AddComponent();
	void Release();

private:
	void CreateExplosionEffect();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
	Engine::CCollider* ptr_sphere_coll_ = nullptr;
	Engine::CGameObject* ptr_effect_ = nullptr;

private:
	float life_time = 0.f;

#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
#endif // _DEBUG
};