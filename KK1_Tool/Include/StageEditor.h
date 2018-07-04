#pragma once
#include "afxwin.h"


// StageEditor 대화 상자입니다.
BEGIN(Engine)
class CGameObject;
class KK1_Font;
END

class MapObject;

class StageEditor : public CPropertyPage
{
	DECLARE_DYNAMIC(StageEditor)

public:
	StageEditor();
	virtual ~StageEditor();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STAGEEDITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

public:
	virtual BOOL OnSetActive();

public:
	afx_msg void OnClickAddObject();
	afx_msg void OnClickDeleteObject();

public:
	void AddMapObject(const std::wstring& obj_key, MapObject* ptr_map_obj);

public:
	void Update(float delta_time);
	void Render();

private:
	void PickObject();
	void RayToViewSpace(Vector3& ray_pos, Vector3& ray_dir);
	void RayToWorldSpace(Vector3& ray_pos, Vector3& ray_dir);
	void ObjectController();

private:
	bool CheckMouseInScreen(const POINT& mouse_pos);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	CListBox resource_list_;
	CListBox object_list_;
	std::unordered_map<std::wstring, MapObject*> map_object_;
	MapObject* ptr_select_object_ = nullptr;

private:
	Engine::KK1_Font* ptr_font_ = nullptr;
	TCHAR mode_text[128] = {};

private: // 컨트롤 변수
	float ctrl_offset_ = 0.f; 
};
