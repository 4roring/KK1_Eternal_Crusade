#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CDynamicMesh;
class CCollider;
class CAnimController;
END

class CChainSword
	: public Engine::CGameObject
{
private:
	explicit CChainSword(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CChainSword();

public:
	Engine::CCollider* ptr_sphere_coll();

public:
	void SetParentMatrix(const Matrix* ptr_parent_matrix);
	void SetHandMatrix(const Matrix* ptr_hand_matrix);
	void set_anim_speed(float anim_speed);

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CChainSword* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CDynamicMesh* ptr_mesh_ = nullptr;
	Engine::CAnimController* ptr_anim_ctrl_ = nullptr;
	Engine::CCollider* ptr_sphere_coll_ = nullptr;

private:
	const Matrix* ptr_parent_matrix_ = nullptr;
	Matrix hand_matrix_;

private:
	float anim_time_ = 0.f;
	float anim_speed_ = 0.f;
};