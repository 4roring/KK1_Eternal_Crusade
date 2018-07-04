// StageEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "StageEditor.h"
#include "afxdialogex.h"

#include "ResourceLoader.h"
#include <memory>

#include "MapObject.h"
#include "Font.h"

// StageEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(StageEditor, CPropertyPage)

StageEditor::StageEditor()
	: CPropertyPage(IDD_STAGEEDITOR)
	, ctrl_offset_(0)
{

}

StageEditor::~StageEditor()
{
}

void StageEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, resource_list_);
	DDX_Control(pDX, IDC_LIST2, object_list_);
	DDX_Text(pDX, IDC_EDIT10, ctrl_offset_);
}


BEGIN_MESSAGE_MAP(StageEditor, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON3, &StageEditor::OnClickAddObject)
	ON_BN_CLICKED(IDC_BUTTON4, &StageEditor::OnClickDeleteObject)
END_MESSAGE_MAP()


// StageEditor 메시지 처리기입니다.


BOOL StageEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	std::auto_ptr<ResourceLoader> auto_ptr_resource_loader(ResourceLoader::Create(ptr_device_, TEXT("../../Client/bin/Resources/Mesh/"), resource_list_));

	Tool()->SetStageEditor(this);
	ptr_font_ = Engine::GraphicDevice()->GetFont(TEXT("바탕"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void StageEditor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 모든 맵 오브젝트 안전 제거
	for (auto& pair : map_object_)
		Safe_Delete(pair.second);

	map_object_.clear();

	CPropertyPage::PostNcDestroy();
}


void StageEditor::OnClickAddObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int select_list = resource_list_.GetCurSel();
	CString select_name;
	resource_list_.GetText(select_list, select_name);

	MapObject* ptr_map_obj = MapObject::Create(ptr_device_, select_name.operator LPCWSTR());
	AddMapObject(select_name.operator LPCWSTR(), ptr_map_obj);

	UpdateData(FALSE);
}

void StageEditor::OnClickDeleteObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int select_list = object_list_.GetCurSel();
	CString select_name;
	object_list_.GetText(select_list, select_name);

	auto iter = map_object_.find(select_name.operator LPCWSTR());
	if (iter != map_object_.end())
	{
		Safe_Delete(iter->second);
		map_object_.erase(iter);
		object_list_.DeleteString(select_list);
	}

	UpdateData(FALSE);
}

void StageEditor::AddMapObject(const std::wstring& obj_key, MapObject * ptr_map_obj)
{
	TCHAR new_obj_key[128] = {};
	auto iter = map_object_.find(obj_key);
	if (iter != map_object_.end())
	{
		int back_number = 0;
		while (iter != map_object_.end())
		{
			wsprintf(new_obj_key, TEXT("%s_%d"), obj_key.c_str(), back_number++);
			iter = map_object_.find(new_obj_key);
		}
	}
	else
		wsprintf(new_obj_key, obj_key.c_str());

	ptr_map_obj->set_object_key(new_obj_key);
	map_object_.emplace(new_obj_key, ptr_map_obj);
	object_list_.AddString(new_obj_key);
}

void StageEditor::Update(float delta_time)
{
	for (auto& pair : map_object_)
		pair.second->Update(delta_time);

	if (Engine::Input()->GetKeyDown(KEY::LBUTTON))
		PickObject();
}

void StageEditor::Render()
{
	for (auto& pair : map_object_)
		pair.second->Render();

	ptr_font_->Render(TEXT("Test"), D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(5.f, 5.f, 0.f));
}

void StageEditor::PickObject()
{
	Vector3 ray_pos;
	Vector3 ray_dir;

	RayToViewSpace(ray_pos, ray_dir);
	RayToWorldSpace(ray_pos, ray_dir);

	std::map<float, MapObject*> pick_object_map;
	for (auto& pair : map_object_)
	{
		float dist = 0.f;
		if (true == pair.second->RaycastToMesh(ray_pos, ray_dir, &dist))
			pick_object_map.emplace(dist, pair.second);
	}

	if (true == pick_object_map.empty())
	{
		ptr_select_object_ = nullptr;
		object_list_.SetCurSel(-1);
	}
	else
	{
		ptr_select_object_ = pick_object_map.begin()->second;
		int cursor_num = object_list_.FindString(0, ptr_select_object_->object_key().c_str());
		object_list_.SetCurSel(cursor_num);
	}
}

void StageEditor::RayToViewSpace(Vector3 & ray_pos, Vector3 & ray_dir)
{
	Matrix mat_proj;
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);
	
	POINT mouse_pos = {};
	GetCursorPos(&mouse_pos);
	ScreenToClient(&mouse_pos);

	Vector3 temp;
	temp.x = (float(mouse_pos.x) / (g_kWinCx >> 1) - 1.f) / mat_proj._11;
	temp.y = (float(-mouse_pos.y) / (g_kWinCy >> 1) + 1.f) / mat_proj._22;
	temp.z = 1.f;

	ray_dir = temp - ray_pos;
	ray_dir = ray_dir.Normalize();
	//D3DXVec3Normalize(&ray_dir, &ray_dir);
}

void StageEditor::RayToWorldSpace(Vector3 & ray_pos, Vector3 & ray_dir)
{
	Matrix inv_mat_view;
	ptr_device_->GetTransform(D3DTS_VIEW, &inv_mat_view);
	D3DXMatrixInverse(&inv_mat_view, nullptr, &inv_mat_view);

	D3DXVec3TransformCoord(&ray_pos, &ray_pos, &inv_mat_view);
	D3DXVec3TransformNormal(&ray_dir, &ray_dir, &inv_mat_view);
}

void StageEditor::ObjectController()
{
}

bool StageEditor::CheckMouseInScreen(const POINT & mouse_pos)
{
	return false;
}

BOOL StageEditor::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Tool()->SetTab(CToolManager::TabID::Stage);
	return CPropertyPage::OnSetActive();
}


