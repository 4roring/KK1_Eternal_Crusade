#include "stdafx.h"
#include "SpaceMarinObserver.h"

CSpaceMarinObserver::CSpaceMarinObserver()
{
}

CSpaceMarinObserver::~CSpaceMarinObserver()
{
}

int CSpaceMarinObserver::GetHp(int ctrl_id)
{
	return hp_[ctrl_id];
}

int CSpaceMarinObserver::GetCurrentCell(int ctrl_id)
{
	return current_cell_[ctrl_id];
}

int CSpaceMarinObserver::GetShiled(int ctrl_id)
{
	return shield_[ctrl_id];
}

void CSpaceMarinObserver::Update(int * shield, int * hp, int * current_cell, int index)
{
	shield_[index] = *shield;
	hp_[index] = *hp;
	current_cell_[index] = *current_cell;
}

CSpaceMarinObserver * CSpaceMarinObserver::Create()
{
	return new CSpaceMarinObserver;
}
