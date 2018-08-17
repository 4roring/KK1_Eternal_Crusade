#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRectTexture;
class CTexture;
END

class CWarBossSkillRange
	: public Engine::CGameObject
{
private:
	explicit CWarBossSkillRange(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CWarBossSkillRange();

private:
	HRESULT Initialize(Vector3* ptr_pos);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CWarBossSkillRange* Create(LPDIRECT3DDEVICE9 ptr_device, Vector3* ptr_pos);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CRectTexture* ptr_buffer_ = nullptr;
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	Vector3* ptr_pos_ = nullptr;
};