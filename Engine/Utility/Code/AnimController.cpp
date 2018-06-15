#include "AnimController.h"

Engine::CAnimController::CAnimController()
{
}

Engine::CAnimController::CAnimController(const CAnimController & rhs)
	: ptr_anim_ctrl_(nullptr)
	, current_track_(rhs.current_track_)
	, new_track_(rhs.new_track_)
	, acc_time_(rhs.acc_time_)
	, period_(rhs.period_)
{
	rhs.ptr_anim_ctrl_->CloneAnimationController(
		rhs.ptr_anim_ctrl_->GetMaxNumAnimationOutputs()
		, rhs.ptr_anim_ctrl_->GetMaxNumAnimationSets()
		, rhs.ptr_anim_ctrl_->GetMaxNumTracks()
		, rhs.ptr_anim_ctrl_->GetMaxNumEvents()
		, &this->ptr_anim_ctrl_);

	this->max_anim_set_ = rhs.ptr_anim_ctrl_->GetMaxNumAnimationSets();
}

Engine::CAnimController::~CAnimController()
{
}

Engine::CAnimController * Engine::CAnimController::CloneAnimController() const
{
	return new CAnimController(*this);
}

LPD3DXANIMATIONCONTROLLER * Engine::CAnimController::GetAnimController()
{
	return &ptr_anim_ctrl_;
}

DOUBLE Engine::CAnimController::GetTrackPosition()
{
	D3DXTRACK_DESC		track_info;
	ZeroMemory(&track_info, sizeof(D3DXTRACK_DESC));

	ptr_anim_ctrl_->GetTrackDesc(current_track_, &track_info);

	return track_info.Position;
}

DOUBLE Engine::CAnimController::GetPeriod()
{
	return period_;
}

void Engine::CAnimController::SetTrackPosition(DOUBLE position)
{
	ptr_anim_ctrl_->SetTrackPosition(current_track_, position);
}

void Engine::CAnimController::SetAnimationTrack(UINT anim_index)
{
	if (anim_index >= max_anim_set_)
		assert(!"anim index overflow");

	if (current_track_ == 0)
		new_track_ = 1;
	else
		new_track_ = 0;

	LPD3DXANIMATIONSET		ptr_anim_set = nullptr;
	ptr_anim_ctrl_->GetAnimationSet(anim_index, &ptr_anim_set);

	period_ = ptr_anim_set->GetPeriod();

	ptr_anim_ctrl_->SetTrackAnimationSet(new_track_, ptr_anim_set);
	ptr_anim_set->Release();

	ptr_anim_ctrl_->UnkeyAllTrackEvents(current_track_);
	ptr_anim_ctrl_->UnkeyAllTrackEvents(new_track_);

	DOUBLE		duration = 0.25;

	//현재트랙 종료
	ptr_anim_ctrl_->KeyTrackEnable(current_track_, FALSE, acc_time_ + duration);
	ptr_anim_ctrl_->KeyTrackSpeed(current_track_, 1.f, acc_time_, duration, D3DXTRANSITION_LINEAR);
	ptr_anim_ctrl_->KeyTrackWeight(current_track_, 0.1f, acc_time_, duration, D3DXTRANSITION_LINEAR);

	//새 트랙 시작
	ptr_anim_ctrl_->SetTrackEnable(new_track_, TRUE);
	ptr_anim_ctrl_->KeyTrackSpeed(new_track_, 1.f, acc_time_, duration, D3DXTRANSITION_LINEAR);
	ptr_anim_ctrl_->KeyTrackWeight(new_track_, 0.9f, acc_time_, duration, D3DXTRANSITION_LINEAR);

	acc_time_ = 0.0;
	current_track_ = new_track_;
	ptr_anim_ctrl_->SetTrackPosition(new_track_, 0.0);
	ptr_anim_ctrl_->ResetTime();
}

void Engine::CAnimController::SetMaxAnimSet()
{
	max_anim_set_ = ptr_anim_ctrl_->GetMaxNumAnimationSets();
}

void Engine::CAnimController::FrameMove(float delta_time)
{
	ptr_anim_ctrl_->AdvanceTime(delta_time, nullptr);
	acc_time_ += delta_time;
}

void Engine::CAnimController::Release()
{
	Safe_Release(ptr_anim_ctrl_);
}

Engine::CAnimController * Engine::CAnimController::Create()
{
	return new CAnimController;
}
