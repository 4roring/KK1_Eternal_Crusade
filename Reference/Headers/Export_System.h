#pragma once

#include "GraphicDevice.h"
#include "TimeManager.h"

BEGIN(Engine)

inline CGraphicDevice* GraphicDevice();
inline CTimeManager* Time();

#include "Export_System.inl"

END