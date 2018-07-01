#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CTexture;
END

class CMapObject
	: public Engine::CGameObject
{
private:
	explicit CMapObject(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CMapObject();
	
private:
	HRESULT Initialize(const std::wstring& mesh_key);

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;

public:
	static CMapObject* Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& mesh_key);

private:
	HRESULT AddComponent(const std::wstring& mesh_key);
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
};