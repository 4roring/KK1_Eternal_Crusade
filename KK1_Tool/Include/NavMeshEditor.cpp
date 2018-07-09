// NavAndLightEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "NavMeshEditor.h"
#include "afxdialogex.h"

#include "NavTri.h"
#include "NavPoint.h"
#include "StageEditor.h"
#include "MapObject.h"

// NavAndLightEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(NavMeshEditor, CPropertyPage)

NavMeshEditor::NavMeshEditor()
	: CPropertyPage(IDD_NAVIEDITOR)
	, check_height_(FALSE)
	, navi_num_(0)
	, cell_option_(0)
	, link_cell_num_(0)
{

}

NavMeshEditor::~NavMeshEditor()
{
}

void NavMeshEditor::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK1, check_height_);
	DDX_Text(pDX, IDC_EDIT1, navi_num_);
	DDX_Text(pDX, IDC_EDIT13, cell_option_);
	DDV_MinMaxInt(pDX, cell_option_, 0, 2);
	DDX_Text(pDX, IDC_EDIT14, link_cell_num_);
}


BEGIN_MESSAGE_MAP(NavMeshEditor, CPropertyPage)
	
	ON_BN_CLICKED(IDC_RADIO1, &NavMeshEditor::OnClick_AddNavPoint)
	ON_BN_CLICKED(IDC_RADIO2, &NavMeshEditor::OnClick_EditNavPoint)
	ON_BN_CLICKED(IDC_RADIO3, &NavMeshEditor::OnClick_DeleteNavMesh)
	ON_BN_CLICKED(IDC_RADIO4, &NavMeshEditor::OnClick_EditNavMeshOption)
	ON_BN_CLICKED(IDC_CHECK1, &NavMeshEditor::OnClick_Check_Height)
	ON_EN_CHANGE(IDC_EDIT13, &NavMeshEditor::Change_NavOption)
	ON_EN_CHANGE(IDC_EDIT14, &NavMeshEditor::Change_LinkNav)
	ON_BN_CLICKED(IDC_BUTTON3, &NavMeshEditor::OnClick_Save)
	ON_BN_CLICKED(IDC_BUTTON4, &NavMeshEditor::OnClick_Load)
END_MESSAGE_MAP()

// MFC Function
BOOL NavMeshEditor::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Tool()->SetNavAndLightEditor(this);
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void NavMeshEditor::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	set_selected_point_.clear();

	for (auto& nav_mesh : vec_nav_mesh_)
		Engine::Safe_Delete(nav_mesh);
	vec_nav_mesh_.clear();

	for (auto& nav_point : vec_nav_point_)
	{
		while (true)
		{
			int ref_cnt = nav_point->Release();
			if (ref_cnt == 0)
			{
				Engine::Safe_Delete(const_cast<NavPoint*>(nav_point));
				break;
			}
		}
	}
	vec_nav_point_.clear();

	CPropertyPage::PostNcDestroy();
}


BOOL NavMeshEditor::OnSetActive()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	Tool()->SetTab(CToolManager::TabID::NaviMesh);

	return CPropertyPage::OnSetActive();
}

void NavMeshEditor::OnClick_AddNavPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ptr_select_navi_ = nullptr;
	Clear_Selected_Points();
	edit_mode_ = EditMode::Add_Point;
}


void NavMeshEditor::OnClick_EditNavPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ptr_select_navi_ = nullptr;
	Clear_Selected_Points();
	edit_mode_ = EditMode::Edit_Point;
}


void NavMeshEditor::OnClick_DeleteNavMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ptr_select_navi_ = nullptr;
	Clear_Selected_Points();
	edit_mode_ = EditMode::Delete_NavMesh;
}


void NavMeshEditor::OnClick_EditNavMeshOption()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ptr_select_navi_ = nullptr;
	Clear_Selected_Points();
	edit_mode_ = EditMode::Edit_NavMesh;
}

void NavMeshEditor::OnClick_Check_Height()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void NavMeshEditor::Change_NavOption()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == ptr_select_navi_) return;
	UpdateData(TRUE);
	ptr_select_navi_->option() = cell_option_;
}


void NavMeshEditor::Change_LinkNav()
{
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == ptr_select_navi_) return;
	if (ptr_select_navi_->option() != 2) return;

	UpdateData(TRUE);
	ptr_select_navi_->link_cell() = link_cell_num_;
}

// MFC Function - End

// class Member Function
void NavMeshEditor::Update(float delta_time)
{
	for (auto& nav_point : set_selected_point_)
		nav_point->Update(delta_time);

	for (auto& nav_point : vec_nav_point_)
		nav_point->Update(delta_time);

	for (auto& nav_tri : vec_nav_mesh_)
		nav_tri->Update(delta_time);

	CheckInput();

	TCHAR text[128] = TEXT("");
	wsprintf(text, TEXT("Waiting Nav Point: %d\n"), set_selected_point_.size());
	Tool()->SetViewText(text);
}

void NavMeshEditor::Render()
{
	for (auto& nav_point : set_selected_point_)
		nav_point->Render();

	auto iter = vec_nav_point_.begin();

	for (; iter != vec_nav_point_.end(); )
	{
		if (1 == (*iter)->reference_count())
		{
			Engine::Safe_Delete(*iter);
			iter = vec_nav_point_.erase(iter);
		}
		else
		{
			(*iter)->Render();
			++iter;
		}
	}

	for (auto& nav_tri : vec_nav_mesh_)
		nav_tri->Render();
}

void NavMeshEditor::CheckInput()
{
	Vector3 ray_pos, ray_dir;
	float dist;

	switch (edit_mode_)
	{
	case EditMode::Add_Point:
		if (Engine::Input()->GetKeyDown(KEY::LBUTTON))
		{
			SetRay(ray_pos, ray_dir);
			if (true == Picking_AddNaviMesh(ray_pos, ray_dir, &dist))
				if (set_selected_point_.size() == 3)
					Add_NaviMesh();
		}
		break;
	case EditMode::Edit_Point:
		if (Engine::Input()->GetKey(KEY::LBUTTON))
		{
			SetRay(ray_pos, ray_dir);
			Picking_EditPoints(ray_pos, ray_dir);
		}
		else
			ptr_edit_point_ = nullptr;
		break;
	case EditMode::Delete_NavMesh:
		if (Engine::Input()->GetKeyDown(KEY::LBUTTON))
		{
			SetRay(ray_pos, ray_dir);
			Picking_DeleteNavMesh(ray_pos, ray_dir);
		}
		break;
	case EditMode::Edit_NavMesh:
		if (Engine::Input()->GetKeyDown(KEY::LBUTTON))
		{
			SetRay(ray_pos, ray_dir);
			Picking_NavMesh(ray_pos, ray_dir);
		}
	}
}

void NavMeshEditor::SetRay(Vector3 & ray_pos, Vector3 & ray_dir)
{
	POINT mouse_pos;
	Engine::Input()->GetMousePos(g_hwnd, &mouse_pos);

	Tool()->ptr_stage_editor()->RayToViewSpace(ray_pos, ray_dir, mouse_pos);
	Tool()->ptr_stage_editor()->RayToWorldSpace(ray_pos, ray_dir);
}

bool NavMeshEditor::Picking_AddNaviMesh(Vector3& ray_pos, Vector3& ray_dir, float* dist)
{
	// 다른 점들을 체크, 점을 픽킹했을 경우 점 정보를 추가.
	for (auto& nav_point : vec_nav_point_)
	{
		if (true == nav_point->RaycastToSphere(ray_pos, ray_dir))
		{
			auto iter = set_selected_point_.find(nav_point);

			set_selected_point_.emplace(nav_point);
			return true;
		}
	}

	// 오브젝트 체크
	if (true == Tool()->ptr_stage_editor()->PickObject(ray_pos, ray_dir, dist))
	{
		Vector3& point_pos = ray_pos + ray_dir * *dist;
		if (FALSE == check_height_)
			point_pos.y = 0.f;
		NavPoint* ptr_nav_point = NavPoint::Create(ptr_device_);
		ptr_nav_point->SetPosition(point_pos);
		set_selected_point_.emplace(ptr_nav_point);
		return true;
	}

	return false;
}

void NavMeshEditor::Add_NaviMesh()
{
	std::array<NavPoint*, 3> nav_point_array;
	int num = 0;

	for (const auto& nav_point : set_selected_point_)
	{
		nav_point_array[num] = nav_point;
		auto iter = std::find(vec_nav_point_.begin(), vec_nav_point_.end(), nav_point);
		if (iter == vec_nav_point_.end())
			vec_nav_point_.emplace_back(nav_point);
		++num;
	}
	set_selected_point_.clear();

	NavTri* ptr_nav = NavTri::Create(ptr_device_, nav_point_array);
	ptr_nav->cell_num() = last_cell_num_;
	++last_cell_num_;
	vec_nav_mesh_.emplace_back(ptr_nav);
}

void NavMeshEditor::Picking_EditPoints(Vector3 & ray_pos, Vector3 & ray_dir)
{
	float dist = 0.f;

	if (nullptr == ptr_edit_point_)
	{
		for (auto& nav_point : vec_nav_point_)
		{
			if (true == nav_point->RaycastToSphere(ray_pos, ray_dir))
				ptr_edit_point_ = nav_point;
		}
	}
	else
	{
		if (true == Tool()->ptr_stage_editor()->PickObject(ray_pos, ray_dir, &dist))
		{
			Vector3& point_pos = ray_pos + ray_dir * dist;
			if (FALSE == check_height_)
				point_pos.y = 0.f;
			ptr_edit_point_->SetPosition(point_pos);
		}
	}
}

void NavMeshEditor::Picking_NavMesh(Vector3& ray_pos, Vector3& ray_dir)
{
	for (auto& nav_mesh : vec_nav_mesh_)
	{
		if (true == nav_mesh->PickNavMesh(ray_pos, ray_dir))
		{
			ptr_select_navi_ = nav_mesh;
			navi_num_ = ptr_select_navi_->cell_num();
			cell_option_ = ptr_select_navi_->option();
			if(cell_option_ == 2)
				link_cell_num_ = ptr_select_navi_->cell_num();
			else
				link_cell_num_ = 0;

			UpdateData(FALSE);
			break;
		}
	}
}

void NavMeshEditor::Picking_DeleteNavMesh(const Vector3& ray_pos, const Vector3& ray_dir)
{
	auto iter = vec_nav_mesh_.begin();
	auto iter_end = vec_nav_mesh_.end();

	for (; iter != iter_end; ++iter)
	{
		if (true == (*iter)->PickNavMesh(ray_pos, ray_dir))
		{
			Engine::Safe_Delete(*iter);
			vec_nav_mesh_.erase(iter);
			break;
		}
	}
}

void NavMeshEditor::Clear_Selected_Points()
{
	for (auto& nav_point : set_selected_point_)
	{
		if (1 == nav_point->reference_count())
			Safe_Delete(const_cast<NavPoint*>(nav_point));
	}
	set_selected_point_.clear();
}

void NavMeshEditor::OnClick_Save()
{
	Tool()->SetFileMode(true);

	CString path = TEXT("");
	Tool()->ptr_stage_editor()->GetPathFromDialog(FALSE, path);

	if (path.IsEmpty()) return;

	HANDLE file = CreateFile(path, GENERIC_WRITE, 0, nullptr
		, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	size_t point_count = vec_nav_point_.size();

	DWORD byte = 0;
	WriteFile(file, &point_count, sizeof(size_t), &byte, nullptr);
	for (size_t i = 0; i < point_count; ++i)
	{
		vec_nav_point_[i]->index() = i;
		WriteFile(file, &vec_nav_point_[i]->index(), sizeof(int), &byte, nullptr);
		WriteFile(file, vec_nav_point_[i]->position(), sizeof(Vector3), &byte, nullptr);
	}

	size_t nav_mesh_count = vec_nav_mesh_.size();
	WriteFile(file, &nav_mesh_count, sizeof(size_t), &byte, nullptr);
	for (auto& nav_mesh : vec_nav_mesh_)
	{
		WriteFile(file, &nav_mesh->GetNavPoint(0)->index(), sizeof(int), &byte, nullptr);
		WriteFile(file, &nav_mesh->GetNavPoint(1)->index(), sizeof(int), &byte, nullptr);
		WriteFile(file, &nav_mesh->GetNavPoint(2)->index(), sizeof(int), &byte, nullptr);

		WriteFile(file, &nav_mesh->cell_num(), sizeof(int), &byte, nullptr);
		WriteFile(file, &nav_mesh->option(), sizeof(int), &byte, nullptr);
		WriteFile(file, &nav_mesh->link_cell(), sizeof(int), &byte, nullptr);
	}
	CloseHandle(file);

	Tool()->SetFileMode(false);
}

void NavMeshEditor::OnClick_Load()
{
	Tool()->SetFileMode(true);

	CString path = TEXT("");
	Tool()->ptr_stage_editor()->GetPathFromDialog(TRUE, path);

	if (path.IsEmpty()) return;

	HANDLE file = CreateFile(path, GENERIC_WRITE, 0, nullptr
		, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	size_t point_count = vec_nav_point_.size();
	DWORD byte = 0;


	Tool()->SetFileMode(false);
}
