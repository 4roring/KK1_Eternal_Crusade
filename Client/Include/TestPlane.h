#pragma once

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
}

class CTestPlane
	: public Engine::CGameObject
{
private:
	explicit CTestPlane(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CTestPlane();

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;

public:
	static CTestPlane* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CVIBuffer* ptr_buffer_ = nullptr;
	Engine::CTexture* ptr_color_texture_ = nullptr;
	Engine::CTexture* ptr_normal_texture_ = nullptr;
};