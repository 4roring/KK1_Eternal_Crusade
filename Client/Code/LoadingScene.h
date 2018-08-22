#pragma once

#include "Scene.h"

BEGIN(Engine)
class KK1_Font;
END

class CLoading;

class CLoadingScene
	: public Engine::CScene
{
private:
	explicit CLoadingScene(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CLoadingScene();

private:
	virtual HRESULT InitScene() override;

public:
	virtual void Update(float delta_time) override;
	virtual void Render();

public:
	static CLoadingScene* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT Add_GameLogic_Layer();
	void Release();

private:
	Engine::CScene* ptr_next_scene_ = nullptr;
	CLoading* ptr_loading_ = nullptr;
	Engine::KK1_Font* ptr_loading_text_ = nullptr;
};