#pragma once

class CSpaceMarinObserver
{
private:
	CSpaceMarinObserver();

public:
	~CSpaceMarinObserver();

public:
	int GetHp(int ctrl_id);
	int GetCurrentCell(int ctrl_id);
	int GetShiled(int ctrl_id);
	int GetBullet(int ctrl_id);
	int GetGranade(int ctrl_id);

public:
	void Update(int * shield, int * hp, int* bullet, int* granade, int * current_cell, int index);

public:
	static CSpaceMarinObserver* Create();

private:
	int shield_[4] = {};
	int hp_[4] = {};
	int bullet_[4] = {};
	int granade_[4] = {};
	int current_cell_[4] = {};
};