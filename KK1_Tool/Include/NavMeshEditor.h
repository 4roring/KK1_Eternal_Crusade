#pragma once


// NavAndLightEditor 대화 상자입니다.

#include <unordered_set>
#include "afxwin.h"

class NavPoint;
class NavTri;

class NavMeshEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(NavMeshEditor)

private:
	enum class EditMode { Add_Point, Edit_Point, Delete_NavMesh, Edit_NavMesh, End };

public:
	NavMeshEditor();
	virtual ~NavMeshEditor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()

private:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual BOOL OnSetActive();

private:
	afx_msg void OnClick_AddNavPoint();
	afx_msg void OnClick_EditNavPoint();
	afx_msg void OnClick_DeleteNavMesh();
	afx_msg void OnClick_EditNavMeshOption();

private:
	afx_msg void OnClick_Check_Height();
	afx_msg void Change_NavOption();
	afx_msg void Change_LinkNav();

public:
	void Update(float delta_time);
	void Render();

private:
	void CheckInput();
	void SetRay(Vector3& ray_pos, Vector3& ray_dir);
	bool Picking_AddNaviMesh(Vector3& ray_pos, Vector3& ray_dir, float* dist);
	void Add_NaviMesh();
	void Picking_EditPoints(Vector3& ray_pos, Vector3& ray_dir);
	void Picking_NavMesh(Vector3& ray_pos, Vector3& ray_dir);
	void Picking_DeleteNavMesh(const Vector3& ray_pos, const Vector3& ray_dir);
	void Clear_Selected_Points();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	EditMode edit_mode_ = EditMode::End;
	std::vector<NavTri*> vec_nav_mesh_;
	NavTri* ptr_select_navi_ = nullptr;
	NavPoint* ptr_edit_point_ = nullptr;
	std::vector<NavPoint*> vec_nav_point_;
	std::unordered_set<NavPoint*> set_selected_point_;

private:
	int last_cell_num_ = 0;

	// Cell Option
private:
	BOOL check_height_;
	int navi_num_;
	int cell_option_;
	int link_cell_num_;
	
public:
	afx_msg void OnClick_Save();
	afx_msg void OnClick_Load();
};
