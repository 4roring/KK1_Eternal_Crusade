#pragma once

#include "Engine_Singleton.h"
#include <functional>

BEGIN(Engine)
class CGameObject;
END

class CSpaceMarinObserver;
class CPlayerCamera;

class CEventManager
	: public Engine::CSingleton<CEventManager>
{
private:
	friend CSingleton;
	CEventManager();
	virtual ~CEventManager();

public:
	void AddEnemy(const TCHAR* object_key, Engine::CGameObject* ptr_object);
	void SetEventCamera(CPlayerCamera* ptr_camera);

public:
	void InitEvent();
	void CheckEvent(float delta_time);

private:
	void EnableObject(int start_num, int end_num);
	void EnableEvent();

private:
	void Release();

private:
	CPlayerCamera* ptr_event_camera_ = nullptr;
	std::vector<Engine::CGameObject*> vec_object_;
	std::list<std::function<bool()>> event_list_;

private:
	CSpaceMarinObserver* ptr_player_observer_ = nullptr;
	int player_cell_num_ = 0;
};

static CEventManager* EventManager() { return CEventManager::GetInstance(); }