#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CSphere;
END

class NavPoint
	: public Engine::CGameObject
{
private:
	explicit NavPoint(LPDIRECT3DDEVICE9 ptr_device);
	
public:
	virtual ~NavPoint();

public:
	uint32 reference_count() { return reference_count_; }
	int& index() { return index_; }
	Vector3& position();

private:
	HRESULT Initialize();

public:
	virtual void Update(float delta_time) override;
	virtual void Render() override;

public:
	uint32 Release();

public:
	static NavPoint* Create(LPDIRECT3DDEVICE9 ptr_device);
	NavPoint* ClonePoint();
	void SetPosition(const Vector3& pos);

public:
	bool RaycastToSphere(Vector3 ray_pos, Vector3 ray_dir);

private:
	HRESULT AddComponent();

private:
	uint32 reference_count_ = 0;
	int index_ = 0;

private:
	Engine::CSphere* ptr_col_sphere_ = nullptr;
};