#pragma once

#include "Engine_Singleton.h"

class CSpaceMarinObserver;
class CEnemyObserver;

class CSubject
	: public Engine::CSingleton<CSubject>
{
private:
	friend CSingleton;
	CSubject();
	virtual ~CSubject();

public:
	void SetSpaceMarinData(int* ptr_shield, int* ptr_hp, int* ptr_current_cell, int ctrl_id);
	void ReserveEnemyData(int size);
	void SetEnemyData(int* ptr_hp, int* ptr_current_cell, int ctrl_id);
	void SetBossHp(int* ptr_hp);
	void SetBossPosition(Vector3* ptr_boss_pos);

public:
	void RegisterObserver(CSpaceMarinObserver*& ptr_observer);
	void RegisterObserver(CEnemyObserver*& ptr_observer);
	void RemoveObserver(CSpaceMarinObserver*& ptr_observer);
	void RemoveObserver(CEnemyObserver*& ptr_observer);
	void Notify();

private:
	void Release();

private: // Data
	int* space_marin_shield_[4] = {};
	int* space_marin_hp_[4] = {};
	int* space_marin_current_cell[4] = {};
	std::vector<int*> vec_enemy_hp_;
	std::vector<int*> vec_enemy_current_cell_;
	int* ptr_boss_hp_ = nullptr;
	Vector3* ptr_boss_pos_ = nullptr;

private: // Observer
	std::list<CSpaceMarinObserver*> space_marin_observer_list_;
	std::list<CEnemyObserver*> enemy_observer_list_;
};

static CSubject* Subject()
{
	return CSubject::GetInstance();
}