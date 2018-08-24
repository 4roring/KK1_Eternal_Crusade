#pragma once

#include "GameObject.h"
#include "Rioreus.h"

BEGIN(Engine)
class CCollider;
END

class CRioreus;

class CPartCollider
	: public Engine::CGameObject
{
private:
	explicit CPartCollider(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CPartCollider();

public:
	void SetAttackState(bool attack_state);
	void SetPartMatrix(const Matrix* ptr_mat_part);

private:
	HRESULT Initialize(const Matrix* mat_part, CRioreus::PartColl part, CRioreus* target);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	virtual void ApplyDamage(int damage) override;

public:
	HRESULT AddComponent();
	void Release();

public:
	static CPartCollider* Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix* mat_part, CRioreus::PartColl part, CRioreus* target);

private:
	Engine::CCollider* ptr_collider_ = nullptr;

private:
	bool attack_state_ = nullptr;
	CRioreus* ptr_target_ = nullptr;
	const Matrix* ptr_mat_part_ = nullptr;
	CRioreus::PartColl part_ = CRioreus::Part_End;
	float damage_time_ = 0.f;
};