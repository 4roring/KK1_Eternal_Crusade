#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRectTexture;
class CTexture;
END

class CFireEffect
	: public Engine::CGameObject
{
private:
	explicit CFireEffect(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CFireEffect();

private:
	HRESULT Initialize(Vector3* pos);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CFireEffect* Create(LPDIRECT3DDEVICE9 ptr_device, Vector3* pos);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CRectTexture* ptr_buffer_ = nullptr;
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	float frame_num_ = 0.f;
	float scene_num_ = 0.f;
	float max_u_ = 0.f;
	float max_v_ = 0.f;

private:
	Vector3* ptr_fix_pos_ = nullptr;
};