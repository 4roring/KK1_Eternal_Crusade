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

public:
	void EnableEvent();

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
	void CameraShaking(float delta_time);
	void EventCamera(float delta_time);

private:
	Engine::CCollider* ptr_sphere_coll_ = nullptr;

private:
	Vector3 next_eye_ = {};
	Vector3 next_at_ = {};

private:
	std::list<Engine::CCollider*> check_list_;
	Vector3 ray_pos_;
	Vector3 ray_dir_;
	int current_cell_index_ = 0;

private:
	Engine::CTransform* ptr_player_transform_ = nullptr;
	CFrustum* ptr_frustum_ = nullptr;

private:
	Matrix inv_mat_proj_;
	Matrix inv_mat_view_;

private:
	float shaking_time_ = 0.f;
	float shaking_acc_time_ = 0.f;
	float shaking_value_ = 0.f;
	float shaking_power_ = 0.f;
	Vector3 shaking_dir_ = {};


private:
	Engine::CTransform* ptr_boss_transform_ = nullptr;
	bool event_mode_ = false;
	float event_time_ = 0.f;


#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
private:
	LPD3DXLINE ptr_line_ = nullptr;
#endif
};