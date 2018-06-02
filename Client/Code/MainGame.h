#pragma once

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
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
};