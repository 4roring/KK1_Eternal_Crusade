#include "stdafx.h"
#include "EventManager.h"

#include "GameObject.h"
#include "SpaceMarinObserver.h"
#include "EnemyObserver.h"
#include "PlayerCamera.h"
#include "LoadingScene.h"


CEventManager::CEventManager()
{
}

CEventManager::~CEventManager()
{
	Release();
}

void CEventManager::AddEnemy(const TCHAR * object_key, Engine::CGameObject * ptr_object)
{
	std::wstring obj_key = object_key;
	int num = obj_key.rfind(TEXT("_"));
	if(0 == obj_key.compare(TEXT("Ork_WarBoss"))) vec_object_[20] = ptr_object;
	else if (0 == obj_key.compare(TEXT("Rioreus"))) vec_object_[0] = ptr_object;
	else if (num == -1) vec_object_[0] = ptr_object;
	else
	{
		num = _ttoi(obj_key.substr(num + 1, obj_key.size()).c_str());
		vec_object_[num + 1] = ptr_object;
	}
}

void CEventManager::SetEventCamera(CPlayerCamera * ptr_camera)
{
	ptr_event_camera_ = ptr_camera;
}

void CEventManager::InitEvent_For_Stage1()
{
	vec_object_.resize(21, nullptr);
	Subject()->RegisterObserver(ptr_player_observer_);
	Subject()->RegisterObserver(ptr_enemy_observer_);

	event_list_.emplace_back([this]()->bool { if (player_cell_num_ == 37){ EnableObject(0, 2); return true; } else return false; });
	event_list_.emplace_back([this]()->bool { if (player_cell_num_ == 63) { EnableObject(3, 13); return true; } else return false; });
	event_list_.emplace_back([this]()->bool { if (player_cell_num_ == 152) { EnableObject(14, 19); return true; } else return false; });
	
	// 보스방. 이벤트카메라 생성.
	event_list_.emplace_back([this]()->bool { if (player_cell_num_ == 222) { EnableObject(20, 20); EnableEvent(); return true; } else return false; });
	event_list_.emplace_back([this]()->bool
	{
		if (ptr_enemy_observer_->boss_hp() < 0)
			return true;

		return false;
	});

	event_state_ = EventState::Stage_1;
}

void CEventManager::InitEvent_For_Stage2()
{
	vec_object_.resize(1, nullptr);
	Subject()->RegisterObserver(ptr_player_observer_);
	Subject()->RegisterObserver(ptr_enemy_observer_);

	// 이벤트카메라 생성.
	event_list_.emplace_back([this]()->bool {  EnableEvent(); return true; });
	event_list_.emplace_back([this]()->bool 
	{  
		if (ptr_enemy_observer_->boss_hp() <= 0)
		{
			SoundSource* ptr_bgm = Sound()->FindSound(TEXT("QuestClear"));
			Sound()->StopSound(Sound()->CHANNEL_BGM);
			Sound()->PlayBGM(ptr_bgm);
			return true;
		}
		
		return false;	
	});

	event_state_ = EventState::Stage_2;
}

void CEventManager::CheckEvent(float delta_time)
{
	if (Engine::Input()->GetKeyDown(KEY::NUM2))
	{
		event_list_.clear();
		Release();
		Engine::GameManager()->SetNextScene(CLoadingScene::Create(Engine::GraphicDevice()->GetDevice()));
		return;
	}

	if (true == event_list_.empty())
	{
		event_time_ += delta_time;
		if (event_time_ >= 5.f)
		{
			if (event_state_ == EventState::Stage_1)
			{
				event_list_.clear();
				Release();
				Engine::GameManager()->SetNextScene(CLoadingScene::Create(Engine::GraphicDevice()->GetDevice()));
			}
			else if (event_state_ == EventState::Stage_2)
				int a = 0;
		}
		return;
	}

	player_cell_num_ = ptr_player_observer_->GetCurrentCell(0);
	if (true == event_list_.front()()) event_list_.pop_front();
}

void CEventManager::EnableObject(int start_num, int end_num)
{
	Engine::CGameObject** pp_start_obj = &vec_object_[0];

	for (int i = start_num; i <= end_num; ++i)
		(*(pp_start_obj + i))->SetActive(true);
}

void CEventManager::EnableEvent()
{
	ptr_event_camera_->EnableEvent();
}

void CEventManager::Release()
{
	if (nullptr != ptr_player_observer_)
		Subject()->RemoveObserver(ptr_player_observer_);

	if (nullptr != ptr_enemy_observer_)
		Subject()->RemoveObserver(ptr_enemy_observer_);
}
