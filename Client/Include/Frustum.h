#pragma once

BEGIN(Engine)
class CCollider;
END

class CFrustum
{
private:
	explicit CFrustum();

public:
	~CFrustum();

private:
	HRESULT Initialize();

public:
	static CFrustum* Create();

public:
	void FrustumToView(const Matrix& inv_mat_proj);
	void FrustumToWorld(const Matrix& inv_mat_view);
	void MakeFrustumPlane();

public:
	bool CheckFrutumToSphere(Engine::CCollider* ptr_sphere_coll);
	bool CheckFrutumToAABB(Engine::CCollider* ptr_aabb_coll);

private:
	Vector3 ori_point_[8] = {};
	Vector3 view_point_[8] = {};
	Vector3 world_point_[8] = {};
	D3DXPLANE plane_[6] = {};
	Vector3 plane_normal_[6] = {};
};