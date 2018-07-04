#pragma once

namespace Engine
{
	class KK1_Font;
}

class CMainGame
{
private:
	CMainGame();
	CMainGame(const CMainGame&) = delete;
	CMainGame& operator=(CMainGame&) = delete;

public:
	~CMainGame();

private:
	HRESULT InitGame();

public:
	void Update();
	void Render();

public:
	static CMainGame* Create();

private:
	void Render_FPS();
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;

private:
	Engine::KK1_Font* ptr_fps_font_ = nullptr;
	int frame_count_ = 0;
	float fps_time_ = 0.f;
	TCHAR fps_[64] = TEXT("");
};