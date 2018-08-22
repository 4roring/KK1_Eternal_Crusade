#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
class KK1_Font;
END

class CSpaceMarinObserver;

class CUI_Weapon
	: public CUI
{
private:
	explicit CUI_Weapon(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CUI_Weapon();

private:
	HRESULT Initialize();

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CUI_Weapon* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CTexture* ptr_pannel_texture_ = nullptr;
	Engine::CTexture* ptr_granade_texture_ = nullptr;
	Engine::CTexture* ptr_gun_texture_ = nullptr;
	Engine::KK1_Font* ptr_font_ = nullptr;

private:
	CSpaceMarinObserver* ptr_player_observer_ = nullptr;
	int bullet_count_ = 0;
	int granade_count_ = 0;
};