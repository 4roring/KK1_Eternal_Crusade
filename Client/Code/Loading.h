#pragma once

BEGIN(Engine)
class CScene;
END

class CLoading
{
public:
	enum class LOADINGID { LOGO, TITLE, STAGE, STAGE_SPECIAL };

private:
	explicit CLoading(LOADINGID loading_id, Engine::CScene** pp_scene_);

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
	HRESULT StageDataLoad(MAINTAINID stage_id, const TCHAR* path);
	HRESULT FindAndLoadMesh(MAINTAINID stage_id, const std::wstring& mesh_key, const std::wstring& path);

public:
	static CLoading* Create(LOADINGID loading_id, Engine::CScene** pp_scene_);
	static uint32 WINAPI LoadingFunction(void* ptr_arg);

private:
	void Release();

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LOADINGID loading_id_;
	HANDLE thread_ = nullptr;

private:
	TCHAR loading_message_[MAX_PATH] = TEXT("");
	bool complete_ = false;

private:
	Engine::CScene** pp_next_scene_ = nullptr;
};