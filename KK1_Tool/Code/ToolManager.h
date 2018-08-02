#pragma once

#include "Engine_Singleton.h"

class CMainFrame;
class StageEditor;
class NavMeshEditor;
class DynamicCamera;

struct Frustum
{
	Vector3 point[8];
	Vector3 world_point[8];
	D3DXPLANE plane[6];
};

class CToolManager
	: public Engine::CSingleton<CToolManager>
{
public:
	enum class TabID { Stage, NaviMesh, Animation, Particle, End };

private:
	friend CSingleton;
	CToolManager();
	virtual ~CToolManager();

public:
	void SetTab(TabID tab_id);
	void SetStageEditor(StageEditor* ptr_stage_editor);
	void SetNavAndLightEditor(NavMeshEditor* ptr_navmesh_editor);
	void SetViewText(const TCHAR* text);

public:
	StageEditor* ptr_stage_editor() { return ptr_stage_editor_; }
	Frustum& frustum() { return frustum_; }

public:
	void Init_ToolManager();
	void Update(float delta_time);
	void Render();
	void Release();

public:
	void BegineScene();
	void EndScene(HWND hwnd);

public:
	void EnableCursor();
	void DisableCursor();

public:
	void SetFileMode(bool file_mode);

private:
	void DrawBoard();

private:
	void FrustumToView();
	void FrustumToWorld();
	void MakeFrustumPlane();

private:
	LPDIRECT3DDEVICE9 ptr_device_;

private:
	CMainFrame* ptr_main_frame_ = nullptr;
	StageEditor* ptr_stage_editor_ = nullptr;
	NavMeshEditor* ptr_nav_n_light_editor_ = nullptr;

private:
	DynamicCamera* ptr_dynamic_camera_ = nullptr;

private:
	Engine::KK1_Font* ptr_font_ = nullptr;
	TCHAR view_text_[128] = {};

private:
	TabID tab_id_ = TabID::End;

private:
	bool show_cursor_ = false;

private:
	ID3DXLine* ptr_line_ = nullptr;

private:
	bool file_mode_ = false;

private:
	Frustum frustum_ = {};
};

static CToolManager* Tool() { return CToolManager::GetInstance(); }