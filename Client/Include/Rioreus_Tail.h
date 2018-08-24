#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
END

class CRioreus_Tail
	: public Engine::CGameObject
{
private:
	explicit CRioreus_Tail(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CRioreus_Tail();

private:
	HRESULT Initialize(const Vector3& start_pos, const Vector3& start_rot);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CRioreus_Tail* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& start_pos, const Vector3& start_rot);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;

private:
	Vector3 forward_vector_ = {};
};