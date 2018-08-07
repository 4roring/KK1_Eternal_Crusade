#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CCollider;
END

class COrk_Klaw
	: public Engine::CGameObject
{
private:
	explicit COrk_Klaw(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~COrk_Klaw();

public:
	Engine::CCollider* ptr_sphere_coll();

public:
	void SetParentMatrix(const Matrix* ptr_parent_matrix);
	void SetHandMatrix(const Matrix* ptr_hand_matrix);

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static COrk_Klaw* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;
	Engine::CCollider* ptr_sphere_coll_ = nullptr;

private:
	const Matrix* ptr_parent_matrix_ = nullptr;
	Matrix hand_matrix_;
};