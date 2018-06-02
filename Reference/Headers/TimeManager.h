#pragma once

#include "Engine_Include.h"
#include "Engine_Singleton.h"

BEGIN(Engine)

class ENGINE_DLL CTimeManager
	: public CSingleton<CTimeManager>
{
private:
	friend CSingleton;
	CTimeManager();
	virtual ~CTimeManager();
	CTimeManager(const CTimeManager&) = delete;
	CTimeManager& operator=(CTimeManager&) = delete;

public:
	float GetDeltaTime() { return delta_time_; }

	void InitTime();
	void SetTime();

private:
	LARGE_INTEGER frame_time_ = {};
	LARGE_INTEGER fix_time_ = {};
	LARGE_INTEGER last_time_ = {};
	LARGE_INTEGER cpu_tick_ = {};

private:
	float time_ = 0.0f;
	float fps_ = 0.0f;
	float delta_time_ = 0.f;
	int fix_frame_ = 0;
};

END