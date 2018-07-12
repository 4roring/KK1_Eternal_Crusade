#pragma once

#include "Scene.h"

class CStage
	: public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CStage();

private:
	virtual HRESULT LateInit() override;

public:
	virtual void Update(float delta_time) override;

public:
	static CStage* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT Add_Environmemt_Layer();
	HRESULT Add_GameLogic_Layer();
	HRESULT Add_Camera_Layer();
	HRESULT Add_UI_Layer();
	HRESULT Add_Light();
	void Release();
};