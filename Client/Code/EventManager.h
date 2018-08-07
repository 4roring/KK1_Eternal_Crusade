#pragma once

#include "Engine_Singleton.h"

class CEventManager
	: public Engine::CSingleton<CEventManager>
{
private:
	friend CSingleton;
	CEventManager();
	virtual ~CEventManager();


};