#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

class CSpaceMarinObserver;

class CUI_Hp
	: public CUI
{
private:
	explicit CUI_Hp(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CUI_Hp();

private:
	HRESULT Initialize();

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CUI_Hp* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	CSpaceMarinObserver* ptr_player_observer_ = nullptr;
	int max_hp_ = 0;
	int hp_ = 0;
};