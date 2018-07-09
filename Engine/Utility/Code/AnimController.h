#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CAnimController
{
private:
	CAnimController();
	CAnimController(const CAnimController& rhs);

public:
	~CAnimController();

public:
	CAnimController* CloneAnimController() const;

public:
	LPD3DXANIMATIONCONTROLLER* GetAnimController();
	DOUBLE GetTrackPosition();
	DOUBLE GetPeriod();

public:
	void SetTrackPosition(DOUBLE position);
	void SetAnimationTrack(uint32 anim_index);
	void SetMaxAnimSet();
	void FrameMove(float delta_time);

private:
	void Release();

public:
	static CAnimController* Create();

private:
	LPD3DXANIMATIONCONTROLLER ptr_anim_ctrl_ = nullptr;

private:
	uint32 max_anim_set_ = 0;
	uint32 current_track_ = 0;
	uint32 new_track_ = 0;
	DOUBLE acc_time_ = 0.0;
	DOUBLE period_ = 0.0;
};

END