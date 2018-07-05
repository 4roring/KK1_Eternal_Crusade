#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
END

class MapObject
	: public Engine::CGameObject
{
private:
	explicit MapObject(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~MapObject();
	
public:
	const std::wstring& mesh_key();
	const std::wstring& object_key();

public:
	void set_object_key(const std::wstring& object_key);

private:
	HRESULT Initialize(const std::wstring& mesh_key);

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;

public:
	bool RaycastToMesh(Vector3& ray_pos, Vector3& ray_dir, float* hit_dir);

public:
	static MapObject* Create(LPDIRECT3DDEVICE9 ptr_device, const std::wstring& mesh_key);

private:
	HRESULT AddComponent(const std::wstring& mesh_key);
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
	std::wstring mesh_key_ = TEXT(" ");
	std::wstring object_key_ = TEXT(" ");
};