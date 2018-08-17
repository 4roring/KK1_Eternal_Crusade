#include "stdafx.h"
#include "ToolManager.h"

#include "MainFrm.h"
#include "EditorTab.h"
#include "StageEditor.h"
#include "NavMeshEditor.h"
#include "ParticleEditor.h"
#include "DynamicCamera.h"
#include "Font.h"

CToolManager::CToolManager()
{

}

CToolManager::~CToolManager()
{
	Release();
}

void CToolManager::SetTab(TabID tab_id)
{
	tab_id_ = tab_id;
}

void CToolManager::SetStageEditor(StageEditor * ptr_stage_editor)
{
	ptr_stage_editor_ = ptr_stage_editor;
}

void CToolManager::SetNavAndLightEditor(NavMeshEditor * ptr_navmesh_editor)
{
	ptr_nav_n_light_editor_ = ptr_navmesh_editor;
}

void CToolManager::SetParticleEditor(ParticleEditor * ptr_particle_editor)
{
	ptr_particle_editor_ = ptr_particle_editor;
}

void CToolManager::SetViewText(const TCHAR * text)
{
	lstrcpy(view_text_, text);
}

void CToolManager::Init_ToolManager()
{
	Engine::GraphicDevice()->InitGraphicDevice(Engine::GraphicDevice()->MODE_WINDOW, g_hwnd, g_kWinCx, g_kWinCy);
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	ptr_dynamic_camera_ = DynamicCamera::Create(ptr_device_, Vector3(0.f, 5.f, -10.f), Vector3(0.f, 0.f, 0.f));
	FrustumToView();

	if (FAILED(D3DXCreateLine(ptr_device_, &ptr_line_)))
		assert(!"Line COM Object Create Failed");
	ptr_line_->SetAntialias(true);

	ptr_main_frame_ = static_cast<CMainFrame*>(AfxGetMainWnd());
	Engine::GraphicDevice()->AddFont(TEXT("¹ÙÅÁ"), 28, 20, FW_NORMAL);
	ptr_font_ = Engine::GraphicDevice()->GetFont(TEXT("¹ÙÅÁ"));
}

void CToolManager::Update(float delta_time)
{
	if (true == file_mode_) return;

	Engine::Input()->SetInputState();
	ptr_dynamic_camera_->Update(delta_time);
	FrustumToWorld();
	MakeFrustumPlane();

	switch (tab_id_)
	{
	case TabID::Stage:
		if (nullptr != ptr_stage_editor_)
			ptr_stage_editor_->Update(delta_time);
		break;
	case TabID::NaviMesh:
		if (nullptr != ptr_nav_n_light_editor_)
			ptr_nav_n_light_editor_->Update(delta_time);
		break;
	case TabID::Animation:
		break;
	case TabID::Particle:
		if (nullptr != ptr_particle_editor_)
			ptr_particle_editor_->Update(delta_time);
		break;
	}
}

void CToolManager::Render()
{
	DrawBoard();
	switch (tab_id_)
	{
	case TabID::Stage:
		if (nullptr != ptr_stage_editor_)
			ptr_stage_editor_->Render();
		break;
	case TabID::NaviMesh:
		if (nullptr != ptr_stage_editor_)
			ptr_stage_editor_->Render();
		if (nullptr != ptr_nav_n_light_editor_)
			ptr_nav_n_light_editor_->Render();
		break;
	case TabID::Animation:
		break;
	case TabID::Particle:
		if (nullptr != ptr_stage_editor_)
			ptr_stage_editor_->Render();
		if (nullptr != ptr_particle_editor_)
			ptr_particle_editor_->Render();
		break;
	}

	ptr_font_->Render(view_text_, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), Vector3(5.f, 5.f, 0.f));
}

void CToolManager::Release()
{
	Engine::Safe_Delete(ptr_dynamic_camera_);
	Engine::Safe_Release(ptr_line_);
	Engine::Time()->DestroyInstance();
	Engine::GameManager()->DestroyInstance();
	Engine::Input()->DestroyInstance();
	Engine::GraphicDevice()->DestroyInstance();
}

void CToolManager::BegineScene()
{
	ptr_device_->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_ARGB(255, 128, 128, 128), 1.f, 0);
	ptr_device_->BeginScene();
}

void CToolManager::EndScene(HWND hwnd)
{
	ptr_device_->EndScene();
	ptr_device_->Present(nullptr, nullptr, hwnd, nullptr);
}

void CToolManager::EnableCursor()
{
	if (false == show_cursor_)
	{
		while (ShowCursor(TRUE) < 0);
		show_cursor_ = true;
	}
}

void CToolManager::DisableCursor()
{
	if (true == show_cursor_)
	{
		while (ShowCursor(FALSE) >= 0);
		show_cursor_ = false;
	}
}

void CToolManager::SetFileMode(bool file_mode)
{
	file_mode_ = file_mode;
}

void CToolManager::DrawBoard()
{
	constexpr int board_max = 11;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	Vector3 ori_vertical_line[2]
		= { Vector3(0.f, 0.f, 0.f), Vector3(10.f, 0.f, 0.f) };
	Vector3 ori_horizontal_line[2]
		= { Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 10.f) };

	Vector3 vertical_line[2]
		= { Vector3(0.f, 0.f, 0.f), Vector3(10.f, 0.f, 0.f) };
	Vector3 horizontal_line[2]
		= { Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 10.f) };

	ptr_line_->SetWidth(2);
	ptr_line_->Begin();
	for (int i = 0; i < board_max; ++i)
	{
		for (int i = 0; i < 2; ++i)
		{
			D3DXVec3TransformCoord(&vertical_line[i], &ori_vertical_line[i], &mat_view);
			D3DXVec3TransformCoord(&horizontal_line[i], &ori_horizontal_line[i], &mat_view);
			
			if (vertical_line[i].z < 0.f)
				vertical_line[i].z = 0.f;

			if (horizontal_line[i].z < 0.f)
				horizontal_line[i].z = 0.f;

			D3DXVec3TransformCoord(&vertical_line[i], &vertical_line[i], &mat_proj);
			D3DXVec3TransformCoord(&horizontal_line[i], &horizontal_line[i], &mat_proj);
		}
		ptr_line_->DrawTransform(vertical_line, 2, nullptr, D3DCOLOR_ARGB(255, 0, 255, 0));
		ptr_line_->DrawTransform(horizontal_line, 2, nullptr, D3DCOLOR_ARGB(255, 0, 255, 0));

		for (int i = 0; i < 2; ++i)
		{
			ori_vertical_line[i] += Vector3(0.f, 0.f, 1.f);
			ori_horizontal_line[i] += Vector3(1.f, 0.f, 0.f);
		}
	}
	ptr_line_->End();
}

void CToolManager::FrustumToView()
{
	Matrix inv_mat_proj;
	D3DXMatrixInverse(&inv_mat_proj, nullptr, &ptr_dynamic_camera_->mat_projection());

	frustum_.point[0] = Vector3(-1.f, 1.f, 0.f);
	frustum_.point[1] = Vector3(1.f, 1.f, 0.f);
	frustum_.point[2] = Vector3(1.f, -1.f, 0.f);
	frustum_.point[3] = Vector3(-1.f, -1.f, 0.f);
		   
	frustum_.point[4] = Vector3(-1.f, 1.f, 1.f);
	frustum_.point[5] = Vector3(1.f, 1.f, 1.f);
	frustum_.point[6] = Vector3(1.f, -1.f, 1.f);
	frustum_.point[7] = Vector3(-1.f, -1.f, 1.f);

	for (auto& point : frustum_.point)
		D3DXVec3TransformCoord(&point, &point, &inv_mat_proj);
}

void CToolManager::FrustumToWorld()
{
	Matrix inv_mat_view;
	D3DXMatrixInverse(&inv_mat_view, nullptr, &ptr_dynamic_camera_->mat_view());

	for (int i = 0; i < 8; ++i)
		D3DXVec3TransformCoord(&frustum_.world_point[i], &frustum_.point[i], &inv_mat_view);
}

void CToolManager::MakeFrustumPlane()
{
	D3DXPlaneFromPoints(&frustum_.plane[0], &frustum_.world_point[1], &frustum_.world_point[5], &frustum_.world_point[6]); // +x
	D3DXPlaneFromPoints(&frustum_.plane[1], &frustum_.world_point[4], &frustum_.world_point[0], &frustum_.world_point[3]); // -x
	D3DXPlaneFromPoints(&frustum_.plane[2], &frustum_.world_point[4], &frustum_.world_point[5], &frustum_.world_point[1]); // +y
	D3DXPlaneFromPoints(&frustum_.plane[3], &frustum_.world_point[3], &frustum_.world_point[2], &frustum_.world_point[6]); // -y
	D3DXPlaneFromPoints(&frustum_.plane[4], &frustum_.world_point[7], &frustum_.world_point[6], &frustum_.world_point[5]); // +z
	D3DXPlaneFromPoints(&frustum_.plane[5], &frustum_.world_point[0], &frustum_.world_point[1], &frustum_.world_point[2]); // -z
}