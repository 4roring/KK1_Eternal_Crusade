#pragma once

#include "GameManager.h"
#include "ComponentManager.h"

BEGIN(Engine)

inline CGameManager* GameManager();
inline CComponentManager* Component();

#include "Export_Utility.inl"

END