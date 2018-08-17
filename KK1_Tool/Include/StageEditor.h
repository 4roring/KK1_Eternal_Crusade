#pragma once
#include "afxwin.h"


// StageEditor ��ȭ �����Դϴ�.
BEGIN(Engine)
class CGameObject;
class KK1_Font;
END

class MapObject;

class StageEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(StageEditor)

private:
	enum class ControlMode { AddMode, Position, Rotation, Scale, End };

public:
	StageEditor();
	virtual ~StageEditor();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STAGEEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual BOOL OnSetActive();

public:
	afx_msg void OnClickAddObject();
	afx_msg void OnClickDeleteObject();
	afx_msg void OnSelectObject();

public:
	void AddMapObject(const std::wstring& obj_key, MapObject* ptr_map_obj);

public:
	void Update(float delta_time);
	void Render();

public:
	void RayToViewSpace(Vector3& ray_pos, Vector3& ray_dir, const POINT& mouse_pos);
	void RayToWorldSpace(Vector3& ray_pos, Vector3& ray_dir);
	// Return the Picking Position
	bool PickObject(Vector3& ray_pos, Vector3& ray_dir, float* out_dist);

private:
	void CheckInput();
	void PickObject();
	void PickAddObject();
	void AddMapObject(const Vector3& obj_pos);
	void ObjectController();

private:
	bool CheckMouseInScreen(const POINT& mouse_pos);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	CListBox resource_list_;
	CListBox object_list_;
	std::map<std::wstring, MapObject*> map_object_;
	MapObject* ptr_select_object_ = nullptr;

private:
	ControlMode ctrl_mode_ = ControlMode::End;
	float mouse_delta_pos_y_ = 0.f;

private: 
	// Control Offset ������ �Լ�
	float pos_offset_; 
	afx_msg void ChangePosOffset();
	float rot_offset_;
	afx_msg void ChangeRotOffset();
	float scale_offset_;
	afx_msg void ChangeScaleOffset();
	// Transform ������ �Լ�
	float pos_x_;
	float pos_y_;
	float pos_z_;
	float rot_x_;
	float rot_y_;
	float rot_z_;
	float scale_x_;
	float scale_y_;
	float scale_z_;
private:
	afx_msg void ChangePosX();
	afx_msg void ChangePosY();
	afx_msg void ChangePosZ();
	afx_msg void ChangeRotX();
	afx_msg void ChangeRotY();
	afx_msg void ChangeRotZ();
	afx_msg void ChangeScaleX();
	afx_msg void ChangeScaleY();
	afx_msg void ChangeScaleZ();

public:
	void GetPathFromDialog(BOOL open_file_dialog, CString& path);

private:
	void ClearMapObject();
	afx_msg void OnClickSave();
	afx_msg void OnClickLoad();
};
