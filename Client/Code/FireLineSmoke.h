#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CLineTexture;
class CTexture;
END

class CFireLineSmoke
	: public Engine::CGameObject
{
private:
	explicit CFireLineSmoke(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CFireLineSmoke();

private:
	HRESULT Initialize(const Vector3& start_pos, const Vector3& end_pos);

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CFireLineSmoke* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& start_pos, const Vector3& end_pos);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CLineTexture* ptr_buffer_ = nullptr;
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	Vector3 start_pos_ = {};
	Vector3 end_pos_ = {};
	float alpha_ = 0.f;
};