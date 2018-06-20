#pragma once

#include "Engine_Singleton.h"

class CToolManager
	: public Engine::CSingleton<CToolManager>
{
private:
	friend CSingleton;
	CToolManager();
	virtual ~CToolManager();

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

private:
	LPDIRECT3DDEVICE9 ptr_device_;
};

static CToolManager* Tool() { return CToolManager::GetInstance(); }