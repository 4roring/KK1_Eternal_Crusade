#pragma once

#include "Engine_Singleton.h"

class CMainFrame;
class StageEditor;
class DynamicCamera;

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
	void DrawBoard();

private:
	LPDIRECT3DDEVICE9 ptr_device_;

private:
	CMainFrame* ptr_main_frame_ = nullptr;
	StageEditor* ptr_stage_editor_ = nullptr;
	DynamicCamera* ptr_dynamic_camera_ = nullptr;

private:
	TabID tab_id_ = TabID::End;

private:
	bool show_cursor_ = false;

private:
	ID3DXLine* ptr_line_ = nullptr;
};

static CToolManager* Tool() { return CToolManager::GetInstance(); }