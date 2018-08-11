#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CCollider;
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
	virtual void LateInit() override;
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CLevelObject* Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& mesh_key);

private:
	HRESULT AddComponent(const std::wstring& mesh_key);
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
	Engine::CCollider* ptr_sphere_coll_ = nullptr;
	Engine::CCollider* ptr_box_coll_ = nullptr;

#ifdef _DEBUG
private:
	Engine::CShader* ptr_debug_shader_ = nullptr;
#endif
};