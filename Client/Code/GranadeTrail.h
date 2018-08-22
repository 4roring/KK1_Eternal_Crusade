#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CTrailColor;
END

class CGranadeTrail
	: public Engine::CGameObject
{
private:
	explicit CGranadeTrail(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CGranadeTrail();

private:
	HRESULT Initialize(const Matrix* ptr_mat_target);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CGranadeTrail* Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix* ptr_mat_target);

private:
	HRESULT AddComponent();
	void Release();

private:
	void PointMemorize(float delta_time);

private:
	Engine::CTrailColor* ptr_trail_ = nullptr;

private:
	const Matrix* ptr_mat_target_ = nullptr;

private:
	std::list<Vector3> point_list_;

private:
	float acc_time_ = 0.f;
	float length_ = 0.f;
};