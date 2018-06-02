#include "TimeManager.h"

Engine::CTimeManager::CTimeManager()
{
}


Engine::CTimeManager::~CTimeManager()
{
}

void Engine::CTimeManager::InitTime()
{
	QueryPerformanceCounter(&frame_time_);
	QueryPerformanceCounter(&fix_time_);
	QueryPerformanceCounter(&last_time_);

	QueryPerformanceFrequency(&cpu_tick_);

	fix_frame_ = 60;

	time_ = float(frame_time_.QuadPart - fix_time_.QuadPart);
	fps_ = float(cpu_tick_.QuadPart / fix_frame_);
}

void Engine::CTimeManager::SetTime()
{
	while (true)
	{
		QueryPerformanceCounter(&frame_time_);
		time_ = float(frame_time_.QuadPart - fix_time_.QuadPart);

		if (fps_ < time_)
		{
			delta_time_ = float(time_ / cpu_tick_.QuadPart);
			fix_time_ = frame_time_;
			break;
		}
	}

	if (frame_time_.QuadPart - last_time_.QuadPart > cpu_tick_.QuadPart)
	{
		QueryPerformanceFrequency(&cpu_tick_);
		last_time_.QuadPart = frame_time_.QuadPart;
	}
}