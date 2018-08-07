#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CCubeTexture;
class CTexture;
END

class CSkybox
	: public Engine::CGameObject
{
private:
	explicit CSkybox(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSkybox();

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CSkybox* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CCubeTexture* ptr_buffer_ = nullptr;
	Engine::CTexture* ptr_texture_ = nullptr;
};