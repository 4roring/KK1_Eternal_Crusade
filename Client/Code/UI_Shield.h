#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

class CSpaceMarinObserver;

class CUI_Shield
	: public CUI
{
private:
	explicit CUI_Shield(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CUI_Shield();

private:
	HRESULT Initialize();

public:
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CUI_Shield* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CTexture* ptr_texture_ = nullptr;

private:
	CSpaceMarinObserver* ptr_player_observer_ = nullptr;
	int max_shield_ = 0;
	int shield_ = 0;
};