#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

enum POINTID { POINT_A, POINT_B, POINT_C, POINT_END };
enum LINEID { LINE_AB, LINE_BC, LINE_CA, LINE_END };
enum NEIGHBORID { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };

class CNavLine;

class CNavCell
{
private:
	explicit CNavCell(LPDIRECT3DDEVICE9 ptr_device
		, const Vector3& point_a, const Vector3& point_b, const Vector3& point_c);

public:
	~CNavCell();

public:
	const Vector3& GetPoint(POINTID point_id);
	const CNavCell* GetNeighbor(NEIGHBORID neighbor_id);
	CNavLine* GetLine(LINEID line_id) const;
	int index() const;
	int option() const;
	int link_cell_index() const;
	Vector3 normal() const;
	Vector3 center() const;

public:
	void SetNeighbor(NEIGHBORID neighbor_id, CNavCell* ptr_neighbor);

private:
	HRESULT InitNavCell(int index, int option, int link_cell_index);

public:
	void Render(LPD3DXLINE ptr_line);

public:
	static CNavCell* Create(LPDIRECT3DDEVICE9 ptr_device
		, const Vector3& point_a, const Vector3& point_b, const Vector3& point_c
		, int index, int option, int link_cell_index);

private:
	void Release();

public:
	bool ComparePoint(const Vector3& first_point, const Vector3& second_point, CNavCell* ptr_neightbor);
	bool CheckPass(const Vector3& pos, const Vector3& dir, NEIGHBORID& neighbor_id);
	bool NeighborNullCheck();
	bool CheckInsideCell(const Vector3& pos);

private:
	CNavLine* ptr_nav_line_[LINE_END] = {};
	Vector3 point_[POINT_END] = {};
	CNavCell* ptr_neighbor_[NEIGHBOR_END] = { nullptr };
	Vector3 normal_ = Vector3();
	Vector3 center_ = Vector3();

private:
	int index_ = 0;
	int option_ = 0;
	int link_cell_index_ = 0;

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
};

END