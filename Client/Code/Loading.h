#pragma once

class CLoading
{
public:
	enum class LOADINGID { LOGO, TITLE, STAGE, STAGE_SPECIAL };

private:
	explicit CLoading(LOADINGID loading_id);

public:
	~CLoading();

public:
	const TCHAR* loading_message();
	const bool complete();

public:
	HRESULT InitLoading();
	HRESULT Stage_Loading();
	HRESULT Stage_Special_Loading();

public:
	static CLoading* Create(LOADINGID loading_id);
	static UINT WINAPI LoadingFunction(void* ptr_arg);

private:
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LOADINGID loading_id_;
	HANDLE thread_ = nullptr;

private:
	TCHAR loading_message_[MAX_PATH] = TEXT("");
	bool complete_ = false;
};