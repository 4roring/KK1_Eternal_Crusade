#pragma once


#include "GameObject.h"

BEGIN(Engine)
class CTrailColor;
END

class CEyeTrail
	: public Engine::CGameObject
{
private:
	explicit CEyeTrail(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CEyeTrail();

private:
	HRESULT Initialize(const Matrix* ptr_target_local, const Matrix* ptr_target_world, bool is_left);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CEyeTrail* Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix* ptr_target_local, const Matrix* ptr_target_world, bool is_left);

private:
	HRESULT AddComponent();
	void Release();

private:
	void PointMemorize(float delta_time);

private:
	Engine::CTrailColor* ptr_trail_ = nullptr;

private:
	Matrix trail_world_;
	const Matrix* ptr_target_local_ = nullptr;
	const Matrix* ptr_target_world_ = nullptr;

private:
	bool is_left_ = false;

private:
	std::list<Vector3> point_list_;

private:
	float acc_time_ = 0.f;
	float length_ = 0.f;
};