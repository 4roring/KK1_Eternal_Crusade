#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CRectTexture;
END

class CUI
	: public Engine::CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CUI();

public:
	virtual void Update(float delta_time) override;
	
protected:
	Vector2 point_ = { 0.f, 0.f };
	float width_ = 0.f;
	float height_ = 0.f;
	Matrix mat_view_;

protected:
	Engine::CRectTexture* ptr_buffer_ = nullptr;
};