#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
END

class CLevelObject
	: public Engine::CGameObject
{
private:
	explicit CLevelObject(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CLevelObject();

private:
	HRESULT Initialize(const std::wstring& mesh_key);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CLevelObject* Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& mesh_key);

private:
	HRESULT AddComponent(const std::wstring& mesh_key);
	void Release();

private:
	HRESULT LoadMesh(const std::wstring& mesh_key, MAINTAINID stage_id);
	HRESULT FindMesh(const std::wstring& mesh_key, const std::wstring& path, MAINTAINID stage_id);

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
};