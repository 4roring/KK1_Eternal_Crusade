#include "stdafx.h"
#include "Subject.h"

#include "SpaceMarinObserver.h"
#include "EnemyObserver.h"

CSubject::CSubject()
{
}

CSubject::~CSubject()
{
	Release();
}

void CSubject::SetSpaceMarinData(int* ptr_shield, int* ptr_hp, int* ptr_current_cell, int ctrl_id)
{
	space_marin_shield_[ctrl_id] = ptr_shield;
	space_marin_hp_[ctrl_id] = ptr_hp;
	space_marin_current_cell[ctrl_id] = ptr_current_cell;
}

void CSubject::ReserveEnemyData(int size)
{
	vec_enemy_current_cell_.resize(size);
	vec_enemy_hp_.resize(size);
}

void CSubject::SetEnemyData(int* ptr_hp, int* ptr_current_cell, int ctrl_id)
{
	vec_enemy_hp_[ctrl_id] = ptr_hp;
	vec_enemy_current_cell_[ctrl_id] = ptr_current_cell;
}

void CSubject::SetBossHp(int * ptr_hp)
{
	ptr_boss_hp_ = ptr_hp;
}

void CSubject::SetBossPosition(Vector3 * ptr_boss_pos)
{
	ptr_boss_pos_ = ptr_boss_pos;
}

void CSubject::RegisterObserver(CSpaceMarinObserver *& ptr_observer)
{
	ptr_observer = CSpaceMarinObserver::Create();
	space_marin_observer_list_.emplace_back(ptr_observer);
}

void CSubject::RegisterObserver(CEnemyObserver *& ptr_observer)
{
	ptr_observer = CEnemyObserver::Create();
	enemy_observer_list_.emplace_back(ptr_observer);
}

void CSubject::RemoveObserver(CSpaceMarinObserver *& ptr_observer)
{
	auto iter = std::find(space_marin_observer_list_.begin(), space_marin_observer_list_.end(), ptr_observer);
	if (iter != space_marin_observer_list_.end())
	{
		Engine::Safe_Delete(ptr_observer);
		space_marin_observer_list_.erase(iter);
	}
}

void CSubject::RemoveObserver(CEnemyObserver *& ptr_observer)
{
	auto iter = std::find(enemy_observer_list_.begin(), enemy_observer_list_.end(), ptr_observer);
	if (iter != enemy_observer_list_.end())
	{
		Engine::Safe_Delete(ptr_observer);
		enemy_observer_list_.erase(iter);
	}
}

void CSubject::Notify()
{
	for (auto& space_marin_observer : space_marin_observer_list_)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (nullptr == space_marin_shield_[i] 
				|| nullptr == space_marin_hp_[i] 
				|| nullptr == space_marin_current_cell[i]) continue;

			space_marin_observer->Update(space_marin_shield_[i], space_marin_hp_[i], space_marin_current_cell[i], i);
		}
	}

	if (nullptr == ptr_boss_hp_) return;

	for (auto& enemy_observer : enemy_observer_list_)
		enemy_observer->Update(vec_enemy_hp_, vec_enemy_current_cell_, *ptr_boss_hp_, *ptr_boss_pos_);
}

void CSubject::Release()
{
	for (auto& space_marin_observer : space_marin_observer_list_)
		Engine::Safe_Delete(space_marin_observer);
	space_marin_observer_list_.clear();

	for (auto& enemy_observer : enemy_observer_list_)
		Engine::Safe_Delete(enemy_observer);
	enemy_observer_list_.clear();
}