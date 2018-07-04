#pragma once

#include "Scene.h"

namespace Engine
{
	class KK1_Font;
}

class CLoading;

class CLogo
	: public Engine::CScene
{
public:
	enum LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC };

private:
	explicit CLogo(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT InitScene();

public:
	virtual ~CLogo();

public:
	virtual void Update(float delta_time) override;
	virtual void Render();
	
public:
	static CLogo* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT Add_GameLogic_Layer();
	void Release();

private:
	CLoading* ptr_loading_ = nullptr;
	Engine::KK1_Font* ptr_loading_text_ = nullptr;
};