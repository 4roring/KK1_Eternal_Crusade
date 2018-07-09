// StageEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "StageEditor.h"
#include "afxdialogex.h"

#include "ResourceLoader.h"
#include <memory>

#include "MapObject.h"
#include "Transform.h"

// StageEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(StageEditor, CPropertyPage)

StageEditor::StageEditor()
	: CPropertyPage(IDD_STAGEEDITOR)
	, pos_offset_(1.f)
	, pos_x_(0.f)
	, pos_y_(0.f)
	, pos_z_(0.f)
	, rot_x_(0.f)
	, rot_y_(0.f)
	, rot_z_(0.f)
	, scale_x_(0.f)
	, scale_y_(0.f)
	, scale_z_(0.f)
	, rot_offset_(45.f)
	, scale_offset_(0.1f)
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
	DDX_Text(pDX, IDC_EDIT10, pos_offset_);
	DDX_Text(pDX, IDC_EDIT1, pos_x_);
	DDX_Text(pDX, IDC_EDIT2, pos_y_);
	DDX_Text(pDX, IDC_EDIT3, pos_z_);
	DDX_Text(pDX, IDC_EDIT4, rot_x_);
	DDX_Text(pDX, IDC_EDIT5, rot_y_);
	DDX_Text(pDX, IDC_EDIT6, rot_z_);
	DDX_Text(pDX, IDC_EDIT7, scale_x_);
	DDX_Text(pDX, IDC_EDIT8, scale_y_);
	DDX_Text(pDX, IDC_EDIT9, scale_z_);
	DDX_Text(pDX, IDC_EDIT11, rot_offset_);
	DDX_Text(pDX, IDC_EDIT12, scale_offset_);
}


BEGIN_MESSAGE_MAP(StageEditor, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON3, &StageEditor::OnClickAddObject)
	ON_BN_CLICKED(IDC_BUTTON4, &StageEditor::OnClickDeleteObject)
	ON_EN_CHANGE(IDC_EDIT10, &StageEditor::ChangePosOffset)
	ON_EN_CHANGE(IDC_EDIT11, &StageEditor::ChangeRotOffset)
	ON_EN_CHANGE(IDC_EDIT12, &StageEditor::ChangeScaleOffset)
	ON_EN_CHANGE(IDC_EDIT1, &StageEditor::ChangePosX)
	ON_EN_CHANGE(IDC_EDIT2, &StageEditor::ChangePosY)
	ON_EN_CHANGE(IDC_EDIT3, &StageEditor::ChangePosZ)
	ON_EN_CHANGE(IDC_EDIT4, &StageEditor::ChangeRotX)
	ON_EN_CHANGE(IDC_EDIT5, &StageEditor::ChangeRotY)
	ON_EN_CHANGE(IDC_EDIT6, &StageEditor::ChangeRotZ)
	ON_EN_CHANGE(IDC_EDIT7, &StageEditor::ChangeScaleX)
	ON_EN_CHANGE(IDC_EDIT8, &StageEditor::ChangeScaleY)
	ON_EN_CHANGE(IDC_EDIT9, &StageEditor::ChangeScaleZ)

	ON_LBN_SELCHANGE(IDC_LIST2, &StageEditor::OnSelectObject)
	ON_BN_CLICKED(IDC_BUTTON1, &StageEditor::OnClickSave)
	ON_BN_CLICKED(IDC_BUTTON2, &StageEditor::OnClickLoad)
END_MESSAGE_MAP()


// StageEditor 메시지 처리기입니다.


BOOL StageEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	std::auto_ptr<ResourceLoader> auto_ptr_resource_loader(ResourceLoader::Create(ptr_device_, TEXT("../../Client/bin/Resources/Mesh/"), resource_list_));

	Tool()->SetStageEditor(this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void StageEditor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 모든 맵 오브젝트 안전 제거
	ClearMapObject();

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
		object_list_.SetCurSel(-1);
		ptr_select_object_ = nullptr;
	}

	UpdateData(FALSE);
}

void StageEditor::OnSelectObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int select_list = object_list_.GetCurSel();
	CString select_name;
	object_list_.GetText(select_list, select_name);

	auto iter = map_object_.find(select_name.operator LPCWSTR());
	if (iter != map_object_.end())
		ptr_select_object_ = iter->second;

	UpdateData(FALSE);
}


BOOL StageEditor::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Tool()->SetTab(CToolManager::TabID::Stage);
	return CPropertyPage::OnSetActive();
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
	CheckInput();

	for (auto& pair : map_object_)
		pair.second->Update(delta_time);
}

void StageEditor::Render()
{
	for (auto& pair : map_object_)
		pair.second->Render();
}

void StageEditor::CheckInput()
{
	if (Engine::Input()->GetKey(KEY::RBUTTON)) return;

	if (Engine::Input()->GetKeyDown(KEY::LBUTTON))
		PickObject();

	if (Engine::Input()->GetKeyDown(KEY::W))
	{
		ctrl_mode_ = (ctrl_mode_ != ControlMode::Position)
			? ControlMode::Position : ControlMode::End;
		Tool()->SetViewText(TEXT("Position Mode"));
	}
	else if (Engine::Input()->GetKeyDown(KEY::E))
	{
		ctrl_mode_ = (ctrl_mode_ != ControlMode::Rotation)
			? ControlMode::Rotation : ControlMode::End;
		Tool()->SetViewText(TEXT("Rotation Mode"));
	}
	else if (Engine::Input()->GetKeyDown(KEY::R))
	{
		ctrl_mode_ = (ctrl_mode_ != ControlMode::Scale)
			? ControlMode::Scale : ControlMode::End;
		Tool()->SetViewText(TEXT("Scale Mode"));
	}
		
	if(ctrl_mode_ != ControlMode::End)
		ObjectController();
	else
		Tool()->SetViewText(TEXT("None"));
}

void StageEditor::PickObject()
{
	POINT mouse_pos;
	Engine::Input()->GetMousePos(g_hwnd, &mouse_pos);

	if (false == CheckMouseInScreen(mouse_pos)) return;

	Vector3 ray_pos;
	Vector3 ray_dir;

	RayToViewSpace(ray_pos, ray_dir, mouse_pos);
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

bool StageEditor::PickObject(Vector3 & ray_pos, Vector3 & ray_dir, float * dist)
{
	std::map<float, MapObject*> pick_object_map;
	for (auto& pair : map_object_)
	{
		if (true == pair.second->RaycastToMesh(ray_pos, ray_dir, dist))
			pick_object_map.emplace(*dist, pair.second);
	}

	if (true == pick_object_map.empty()) return false;
	*dist = pick_object_map.begin()->first;
	return true;
}

void StageEditor::RayToViewSpace(Vector3 & ray_pos, Vector3 & ray_dir, const POINT& mouse_pos)
{
	Matrix mat_proj;
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);
	
	Vector3 temp;
	temp.x = (float(mouse_pos.x) / (g_kWinCx >> 1) - 1.f) / mat_proj._11;
	temp.y = (float(-mouse_pos.y) / (g_kWinCy >> 1) + 1.f) / mat_proj._22;
	temp.z = 1.f;

	ray_pos = Vector3();
	ray_dir = temp - ray_pos;
	ray_dir = ray_dir.Normalize();
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
	if (nullptr == ptr_select_object_) return;

	constexpr float dist = 2.f;
	mouse_delta_pos_y_ = (float)Engine::Input()->GetDIMouseMove(Engine::CInputManager::DIM_Y);

	switch (ctrl_mode_)
	{
	case ControlMode::Position:
		if (Engine::Input()->GetKey(KEY::X))
		{
			if(mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->position().x -= pos_offset_;
			else if(mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->position().x += pos_offset_;
		}
		else if (Engine::Input()->GetKey(KEY::Y))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->position().y -= pos_offset_;
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->position().y += pos_offset_;
		}
		else if (Engine::Input()->GetKey(KEY::Z))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->position().z -= pos_offset_;
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->position().z += pos_offset_;
		}
		pos_x_ = ptr_select_object_->transform()->position().x;
		pos_y_ = ptr_select_object_->transform()->position().y;
		pos_z_ = ptr_select_object_->transform()->position().z;
		break;
	case ControlMode::Rotation:
		if (Engine::Input()->GetKey(KEY::X))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->rotation().x -= D3DXToRadian(rot_offset_);
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->rotation().x += D3DXToRadian(rot_offset_);
		}
		else if (Engine::Input()->GetKey(KEY::Y))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->rotation().y -= D3DXToRadian(rot_offset_);
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->rotation().y += D3DXToRadian(rot_offset_);
		}
		else if (Engine::Input()->GetKey(KEY::Z))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->rotation().z -= D3DXToRadian(rot_offset_);
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->rotation().z += D3DXToRadian(rot_offset_);
		}
		rot_x_ = D3DXToDegree(ptr_select_object_->transform()->rotation().x);
		rot_y_ = D3DXToDegree(ptr_select_object_->transform()->rotation().y);
		rot_z_ = D3DXToDegree(ptr_select_object_->transform()->rotation().z);
		break;
	case ControlMode::Scale:
		if (Engine::Input()->GetKey(KEY::X))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->scale().x -= scale_offset_;
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->scale().x += scale_offset_;
		}
		else if (Engine::Input()->GetKey(KEY::Y))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->scale().y -= scale_offset_;
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->scale().y += scale_offset_;
		}
		else if (Engine::Input()->GetKey(KEY::Z))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->scale().z -= scale_offset_;
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->scale().z += scale_offset_;
		}
		else if (Engine::Input()->GetKey(KEY::A))
		{
			if (mouse_delta_pos_y_ > dist)
				ptr_select_object_->transform()->scale() -= scale_offset_;
			else if (mouse_delta_pos_y_ < -dist)
				ptr_select_object_->transform()->scale() += scale_offset_;
		}
		scale_x_ = ptr_select_object_->transform()->scale().x;
		scale_y_ = ptr_select_object_->transform()->scale().y;
		scale_z_ = ptr_select_object_->transform()->scale().z;
		break;
	}
	UpdateData(FALSE);
}


bool StageEditor::CheckMouseInScreen(const POINT & mouse_pos)
{
	if (mouse_pos.x < 0) return false;
	else if (mouse_pos.x > g_kWinCx) return false;
	else if (mouse_pos.y < 0) return false;
	else if (mouse_pos.y > g_kWinCy) return false;

	return true;
}

void StageEditor::ChangePosOffset()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void StageEditor::ChangeRotOffset()
{
	UpdateData(TRUE);
}


void StageEditor::ChangeScaleOffset()
{
	UpdateData(TRUE);
}

void StageEditor::ChangePosX()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->position().x = pos_x_;
	UpdateData(FALSE);
}

void StageEditor::ChangePosY()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->position().y = pos_y_;
	UpdateData(FALSE);
}

void StageEditor::ChangePosZ()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->position().z = pos_z_;
	UpdateData(FALSE);
}

void StageEditor::ChangeRotX()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->rotation().x = D3DXToRadian(rot_x_);
	UpdateData(FALSE);
}

void StageEditor::ChangeRotY()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->rotation().y = D3DXToRadian(rot_y_);
	UpdateData(FALSE);
}

void StageEditor::ChangeRotZ()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->rotation().z = D3DXToRadian(rot_z_);
	UpdateData(FALSE);
}


void StageEditor::ChangeScaleX()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->scale().x = scale_x_;
	UpdateData(FALSE);
}


void StageEditor::ChangeScaleY()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);
	ptr_select_object_->transform()->scale().y = scale_y_;
	UpdateData(FALSE);
}


void StageEditor::ChangeScaleZ()
{
	if (ctrl_mode_ != ControlMode::End) return;

	UpdateData(TRUE);	
	ptr_select_object_->transform()->scale().z = scale_z_;
	UpdateData(FALSE);
}

void StageEditor::ClearMapObject()
{
	for (auto& pair : map_object_)
		Safe_Delete(pair.second);

	map_object_.clear();
}

void StageEditor::GetPathFromDialog(BOOL open_file_dialog, CString & path)
{
	CFileDialog dlg(open_file_dialog, TEXT("dat"), TEXT("*.dat")
		, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, TEXT("*.dat||"), this);

	HMODULE module = GetModuleHandle(nullptr);

	TCHAR full_path[MAX_PATH] = TEXT("");
	DWORD error_code = GetModuleFileNameW(module, full_path, MAX_PATH);
	VERIFY(error_code);
	PathRemoveFileSpecW(full_path);
	PathRemoveFileSpecW(full_path);
	PathRemoveFileSpecW(full_path);

	wsprintf(full_path, TEXT("%s\\Client\\bin\\Data\\"), full_path);
	
	dlg.m_ofn.lpstrInitialDir = full_path;
	if (dlg.DoModal() == IDCANCEL)
		return;

	path = dlg.GetPathName();
}

void StageEditor::OnClickSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Tool()->SetFileMode(true);

	CString path;
	GetPathFromDialog(FALSE, path);

	if (path.IsEmpty()) return;

	HANDLE file = CreateFile(path, GENERIC_WRITE, 0, nullptr
		, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	size_t object_count = map_object_.size();
	TCHAR mesh_key[128] = TEXT("");
	TCHAR object_key[128] = TEXT("");

	DWORD byte = 0;

	WriteFile(file, &object_count, sizeof(size_t), &byte, nullptr);

	for (auto& pair : map_object_)
	{
		// Mesh Component Key, Object Key
		wsprintf(mesh_key, pair.second->mesh_key().c_str());
		wsprintf(object_key, pair.second->object_key().c_str());
		WriteFile(file, mesh_key, sizeof(mesh_key), &byte, nullptr);
		WriteFile(file, object_key, sizeof(object_key), &byte, nullptr);

		// Transform
		WriteFile(file, pair.second->transform()->position(), sizeof(Vector3), &byte, nullptr);
		WriteFile(file, pair.second->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
		WriteFile(file, pair.second->transform()->scale(), sizeof(Vector3), &byte, nullptr);
	}
	CloseHandle(file);

	Tool()->SetFileMode(false);
}


void StageEditor::OnClickLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Tool()->SetFileMode(true);

	CString path;
	GetPathFromDialog(TRUE, path);

	if (path.IsEmpty()) return;

	object_list_.ResetContent();
	ClearMapObject();

	HANDLE file = CreateFile(path, GENERIC_READ, 0, nullptr
		, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	size_t object_count = 0;
	MapObject* ptr_obj = nullptr;
	TCHAR mesh_key[128] = TEXT("");
	TCHAR object_key[128] = TEXT("");

	DWORD byte = 0;
	
	ReadFile(file, &object_count, sizeof(size_t), &byte, nullptr);
	for (size_t i = 0; i < object_count; ++i)
	{
		ReadFile(file, mesh_key, sizeof(mesh_key), &byte, nullptr);
		ReadFile(file, object_key, sizeof(object_key), &byte, nullptr);
		ptr_obj = MapObject::Create(ptr_device_, mesh_key);
		ptr_obj->set_object_key(object_key);

		ReadFile(file, ptr_obj->transform()->position(), sizeof(Vector3), &byte, nullptr);
		ReadFile(file, ptr_obj->transform()->rotation(), sizeof(Vector3), &byte, nullptr);
		ReadFile(file, ptr_obj->transform()->scale(), sizeof(Vector3), &byte, nullptr);
		map_object_.emplace(object_key, ptr_obj);
		object_list_.AddString(object_key);
	}
	CloseHandle(file);

	Tool()->SetFileMode(false);
}
