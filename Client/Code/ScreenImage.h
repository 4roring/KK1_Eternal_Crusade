#pragma once

#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTexture;
	class CShader;
}

class CScreenImage
	: public Engine::CGameObject
{
private:
	explicit CScreenImage(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CScreenImage();

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;
	
public:
	static CScreenImage* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CVIBuffer* ptr_buffer_ = nullptr;
	Engine::CTexture* ptr_texture_ = nullptr;
	Engine::CShader* ptr_shader_ = nullptr;
};