#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CSphere;
END

class COrk_HeavyGun
	: public Engine::CGameObject
{
private:
	explicit COrk_HeavyGun(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~COrk_HeavyGun();

public:
	Vector3& GetFirePos(int index) const;

public:
	void SetParentMatrix(const Matrix* ptr_parent_matrix);
	void SetBackMatrix(const Matrix* ptr_back_matrix);

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static COrk_HeavyGun* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;

private:
	const Matrix* ptr_parent_matrix_ = nullptr;
	Matrix back_matrix_;

private:
	Matrix mat_muzzle_of_gun_[2];

private:
	Vector4 color_[3] = {};

#ifdef _DEBUG
private:
	Engine::CSphere* ptr_debug_fire_pos_ = nullptr;
#endif
};