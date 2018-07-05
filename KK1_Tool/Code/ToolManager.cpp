#include "stdafx.h"
#include "ToolManager.h"

#include "MainFrm.h"
#include "EditorTab.h"
#include "StageEditor.h"
#include "DynamicCamera.h"

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

void CToolManager::Init_ToolManager()
{
	Engine::GraphicDevice()->InitGraphicDevice(Engine::GraphicDevice()->MODE_WINDOW, g_hwnd, g_kWinCx, g_kWinCy);
	ptr_device_ = Engine::GraphicDevice()->GetDevice();
	ptr_dynamic_camera_ = DynamicCamera::Create(ptr_device_, Vector3(0.f, 5.f, -10.f), Vector3(0.f, 0.f, 0.f));

	if (FAILED(D3DXCreateLine(ptr_device_, &ptr_line_)))
		assert(!"Line COM Object Create Failed");
	ptr_line_->SetAntialias(true);

	ptr_main_frame_ = static_cast<CMainFrame*>(AfxGetMainWnd());
}

void CToolManager::Update(float delta_time)
{
	if (true == file_mode_) return;

	Engine::Input()->SetInputState();
	ptr_dynamic_camera_->Update(delta_time);

	switch (tab_id_)
	{
	case TabID::Stage:
	case TabID::NaviMesh:
		if (nullptr != ptr_stage_editor_)
			ptr_stage_editor_->Update(delta_time);
		break;
	case TabID::Animation:
		break;
	case TabID::Particle:
		break;
	}
}

void CToolManager::Render()
{
	DrawBoard();
	switch (tab_id_)
	{
	case TabID::Stage:
	case TabID::NaviMesh:
		if (nullptr != ptr_stage_editor_)
			ptr_stage_editor_->Render();
		break;
	case TabID::Animation:
		break;
	case TabID::Particle:
		break;
	}
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


