#pragma once

class NavPoint;

class NavTri
{
private:
	NavTri(LPDIRECT3DDEVICE9 ptr_device);

public:
	~NavTri();

private:
	HRESULT Initialize(const std::array<NavPoint*, 3>& nav_point_array);

public:
	int& cell_num() { return cell_num_; }
	int& option() { return option_; }
	int& link_cell() { return link_cell_; }
	NavPoint* GetNavPoint(int i) const { return nav_point_array_[i]; }

public:
	void Update(float delta_time);
	void Render();

private:
	void Release();

public:
	static NavTri* Create(LPDIRECT3DDEVICE9 ptr_device, const std::array<NavPoint*, 3>& nav_point_array);

public:
	bool PickNavMesh(const Vector3& ray_pos, const Vector3& ray_dir);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXLINE ptr_line_ = nullptr;

private:
	std::array<NavPoint*, 3> nav_point_array_;
	Vector3 viewport_nav_point_[4];
	int cell_num_ = 0;
	int option_ = 0;
	int link_cell_ = 0;
};