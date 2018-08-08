#pragma once

class CEnemyObserver
{
private:
	CEnemyObserver();

public:
	~CEnemyObserver();

public:
	int GetHp(int ctrl_id);
	int GetCurrentCell(int ctrl_id);
	int boss_hp();
	const Vector3& boss_pos();

public:
	void Update(const std::vector<int*>& vec_enemy_hp, const std::vector<int*>& vec_enemy_current_cell, int boss_hp, const Vector3& boss_pos);

public:
	static CEnemyObserver* Create();

private:
	std::vector<int> vec_enemy_hp_;
	std::vector<int> vec_enemy_current_cell_;
	int boss_hp_ = {};
	Vector3 boss_pos_ = {};
};