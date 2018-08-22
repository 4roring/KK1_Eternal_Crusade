#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

class CUI_Aim
	: public CUI
{
private:
	explicit CUI_Aim(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CUI_Aim();

public:
	void set_aim_state(int state);

private:
	HRESULT Initialize();

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CUI_Aim* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	int aim_state_ = 0;
};