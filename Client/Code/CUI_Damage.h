#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

class CUI_Damage
	: public CUI
{
private:
	explicit CUI_Damage(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CUI_Damage();

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
	static CUI_Damage* Create(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual void OnEnable() override;

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	float alpha_ = 0;
};