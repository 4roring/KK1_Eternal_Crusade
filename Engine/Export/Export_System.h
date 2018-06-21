#pragma once

#include "GraphicDevice.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "FontManager.h"

BEGIN(Engine)

inline CGraphicDevice* GraphicDevice();
inline CTimeManager* Time();
inline CInputManager* Input();

#include "Export_System.inl"

END