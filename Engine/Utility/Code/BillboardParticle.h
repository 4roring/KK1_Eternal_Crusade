#pragma once

#include "Particle.h"

BEGIN(Engine)

class CRectTexture;

class CBillboardParticle
	: public CParticle
{
private:
	enum class Type { Normal, UV, End };

private:
	explicit CBillboardParticle(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CBillboardParticle();

private:
	HRESULT Initialize(int buffer_stage_id, const std::wstring& buffer_key, bool is_uv_anim, bool uv_anim_loop, float max_u, float max_v);

public:
	virtual void Update(float delta_time, const Vector3& position, const Vector3& rotation, const Vector3& scale, const Vector3& camera_pos) override;
	virtual void Render(LPD3DXEFFECT ptr_effect, const Matrix* mat_parent) override;

private:
	void Release();

public:
	static CBillboardParticle* Create(LPDIRECT3DDEVICE9 ptr_device, int buffer_stage_id, const std::wstring& buffer_key, bool is_uv_anim, bool uv_anim_loop, float max_u, float max_v);

private:
	Vector3 camera_pos_ = {};
	CRectTexture* ptr_buffer_ = nullptr;
	Type type_ = Type::End;
	float frame_num_ = 0.f;
	float scene_num_ = 0.f;
	float max_u_ = 0.f;
	float max_v_ = 0.f;
	bool uv_anim_loop_ = false;
};

END