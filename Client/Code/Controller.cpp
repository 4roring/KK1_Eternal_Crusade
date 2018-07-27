#include "stdafx.h"
#include "Controller.h"

CController::CController()
{
}

CController::~CController()
{
}

void CController::LateInit()
{
	late_init_ = true;
}

void CController::Update(float delta_time)
{
	if (false == late_init_)
		LateInit();
}
