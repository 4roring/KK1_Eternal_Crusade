#pragma once

#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CTexture;
}

class CTestPlane2
	: public Engine::CGameObject
{
private:
	explicit CTestPlane2(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CTestPlane2();

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;

public:
	static CTestPlane2* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
};