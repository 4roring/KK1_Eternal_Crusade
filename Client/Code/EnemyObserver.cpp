#include "stdafx.h"
#include "EnemyObserver.h"

CEnemyObserver::CEnemyObserver()
{
}

CEnemyObserver::~CEnemyObserver()
{
}

int CEnemyObserver::GetHp(int ctrl_id)
{
	if (vec_enemy_hp_.size() <= (size_t)ctrl_id) return -1;

	return vec_enemy_hp_[ctrl_id];
}

int CEnemyObserver::GetCurrentCell(int ctrl_id)
{
	if (vec_enemy_current_cell_.size() <= (size_t)ctrl_id) return -1;

	return vec_enemy_current_cell_[ctrl_id];
}

int CEnemyObserver::boss_hp()
{
	return boss_hp_;
}

const Vector3 & CEnemyObserver::boss_pos()
{
	return boss_pos_;
}

void CEnemyObserver::Update(const std::vector<int*>& vec_enemy_hp, const std::vector<int*>& vec_enemy_current_cell, int boss_hp, const Vector3 & boss_pos)
{
	vec_enemy_hp_.clear();
	vec_enemy_current_cell_.clear();

	for (int* hp : vec_enemy_hp)
		vec_enemy_hp_.emplace_back(*hp);

	for (int* current_cell : vec_enemy_current_cell)
		vec_enemy_current_cell_.emplace_back(*current_cell);

	boss_hp_ = boss_hp;
	boss_pos_ = boss_pos;
}

CEnemyObserver * CEnemyObserver::Create()
{
	return new CEnemyObserver;
}
