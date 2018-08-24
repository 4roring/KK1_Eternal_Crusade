#pragma once

#include "Engine_Singleton.h"
#include <functional>

BEGIN(Engine)
class CGameObject;
END

class CSpaceMarinObserver;
class CPlayerCamera;
class CEnemyObserver;

class CEventManager
	: public Engine::CSingleton<CEventManager>
{
private:
	enum EventState { Stage_1, Stage_2, End };

private:
	friend CSingleton;
	CEventManager();
	virtual ~CEventManager();

public:
	void AddEnemy(const TCHAR* object_key, Engine::CGameObject* ptr_object);
	void SetEventCamera(CPlayerCamera* ptr_camera);

public:
	void InitEvent_For_Stage1();
	void InitEvent_For_Stage2();
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
	CEnemyObserver* ptr_enemy_observer_ = nullptr;
	int player_cell_num_ = 0;
	float event_time_ = 0.f;
	EventState event_state_ = EventState::End;
};

static CEventManager* EventManager() { return CEventManager::GetInstance(); }