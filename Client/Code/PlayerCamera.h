#pragma once

#include "Camera.h"

BEGIN(Engine)
class CTransform;
class CCollider;
class CShader;
END

class CFrustum;

class CPlayerCamera
	: public Engine::CCamera
{
private:
	explicit CPlayerCamera(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CPlayerCamera();

private:
	HRESULT InitCamera(MAINTAINID stage_id);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CPlayerCamera* Create(LPDIRECT3DDEVICE9 ptr_device, MAINTAINID stage_id);

public:
	virtual void SetView(const Vector3& eye, const Vector3& at) override;
	virtual void SetProjection(float fov_y, float aspect, float z_near, float z_far) override;

private:
	HRESULT AddComponent();
	void Release();

public:
	HRESULT GetComponent(MAINTAINID stage_id);

private:
	Engine::CCollider* ptr_sphere_coll_ = nullptr;

private:
	std::list<Engine::CCollider*> check_list_;
	Vector3 ray_pos_;
	Vector3 ray_dir_;

private:
	Engine::CTransform* ptr_player_transform_ = nullptr;
	CFrustum* ptr_frustum_ = nullptr;


#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
private:
	LPD3DXLINE ptr_line_ = nullptr;
#endif
};