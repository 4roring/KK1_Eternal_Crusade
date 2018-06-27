#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera
	: public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CCamera();

public:
	Vector3& eye();
	Vector3& at();
	Matrix& mat_view();
	Matrix& mat_projection();

public:
	void SetView(const Vector3& eye, const Vector3& at);
	void SetProjection(float fov_y, float aspect, float z_near, float z_far);

protected:
	virtual HRESULT Initialize() override;

public:
	virtual void Update(float delta_time);

protected:
	void Release();

private:
	void Invalidate_View();
	void Invalidate_Projection();

private:
	struct CameraInfo* ptr_camera_info_;
};

END