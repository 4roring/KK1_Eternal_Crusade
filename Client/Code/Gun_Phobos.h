#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CSphere;
END

class CGun_Phobos
	: public Engine::CGameObject
{
private:
	explicit CGun_Phobos(LPDIRECT3DDEVICE9 ptr_device);
	
public:
	virtual ~CGun_Phobos();

public:
	Vector3& GetFirePos() const;

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
	static CGun_Phobos* Create(LPDIRECT3DDEVICE9 ptr_device);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CStaticMesh* ptr_mesh_ = nullptr;

private:
	const Matrix* ptr_parent_matrix_ = nullptr;
	Matrix hand_matrix_;

private:
	Matrix mat_muzzle_of_gun_;

#ifdef _DEBUG
private:
	Engine::CSphere* ptr_debug_fire_pos_ = nullptr;
#endif
};