#include "stdafx.h"
#include "Rioreus_Fire.h"

#include "Rioreus.h"
#include "Transform.h"
#include "AnimController.h"
#include "FireBall.h"
#include "Rioreus_Explosion.h"

void CRioreus_Fire::InitState(CRioreus * ptr_target, Engine::CTransform * ptr_transform, Engine::CAnimController * ptr_anim_ctrl)
{
	CRioreus_State::InitState(ptr_target, ptr_transform, ptr_anim_ctrl);
	ptr_sound_fire_ = Sound()->FindSound(TEXT("Rioreus_Fire"));
	ptr_sound_part_fire_ = Sound()->FindSound(TEXT("Rioreus_Part_Fire"));
	ptr_sound_explosion_ = Sound()->FindSound(TEXT("Explosion"));
}

void CRioreus_Fire::Reset()
{
	CRioreus_State::Reset();
	fire_state_ = State::End;
}

void CRioreus_Fire::Update(float delta_time)
{
	switch (fire_state_)
	{
	case State::Setup: SetUpFireState();
		break;
	case State::Back_Explosion: Fire_Back_Explosion(delta_time);
		break;
	case State::Triple: Fire_Triple(delta_time);
		break;
	case State::Fire: Fire(delta_time);
		break;
	case State::End:
		condition_ = 0.f;
		ptr_target_->SetState(CRioreus::State::Idle);
		fire_state_ = State::Setup;
		break;
	}
}

void CRioreus_Fire::SetUpFireState()
{
	const float square_near = 15.f * 15.f;
	const float square_mid = 25.f * 25.f;
	const float square_dist = ((*Subject()->player_pos()) - ptr_transform_->position()).Magnitude();

	if (square_dist < square_near)
	{
		ptr_anim_ctrl_->SetAnimationTrack("Fire_Back_Explosion");
		fire_state_ = State::Back_Explosion;
	}
	else if (square_dist < square_mid)
	{
		ptr_anim_ctrl_->SetAnimationTrack("Fire_Triple");
		fire_state_ = State::Triple;
	}
	else
	{
		ptr_anim_ctrl_->SetAnimationTrack("Fire");
		fire_state_ = State::Fire;
	}

	const float next_anim_speed = (true == ptr_target_->rage()) ? 2.f : 1.6f;
	ptr_target_->SetAnimSpeed(next_anim_speed);
	Sound()->PlaySound(ptr_sound_fire_, Sound()->CHANNEL_ENEMY_ATTACK);
}

void CRioreus_Fire::Fire_Back_Explosion(float delta_time)
{
	if (false == ptr_anim_ctrl_->CheckCurrentAnimationEnd(7.8))
	{
		int option = -1;
		int& cell_index = ptr_target_->current_cell_index();
		const float back_move_speed = -3.f;

		ptr_transform_->move_dir() = Vector3(ptr_transform_->Forward().Normalize()) * back_move_speed * delta_time;
		cell_index = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), ptr_transform_->move_dir(), cell_index, option);
		ptr_target_->SetAnimSpeed(2.f);
	}

	if (condition_ == 0.f && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(5.0))
	{
		CreateExplosion();
		++condition_;
	}

	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		fire_state_ = State::End;
		condition_ = 0.f;
	}
}

void CRioreus_Fire::Fire_Triple(float delta_time)
{
	if (condition_ == 0.f && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(9.7))
	{
		CreateFire();
		condition_ = 1.f;
	}
	else if (condition_ == 1.f && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(8.6))
	{
		CreateFire();
		Sound()->StopSound(Sound()->CHANNEL_ENEMY_ATTACK);
		Sound()->PlaySound(ptr_sound_part_fire_, Sound()->CHANNEL_ENEMY_ATTACK);
		condition_ = 2.f;
	}
	else if (condition_ == 2.f && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(7.1))
	{
		CreateFire();
		Sound()->StopSound(Sound()->CHANNEL_ENEMY_ATTACK);
		Sound()->PlaySound(ptr_sound_part_fire_, Sound()->CHANNEL_ENEMY_ATTACK);
		condition_ = 3.f;
	}

	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
		Reset();
}

void CRioreus_Fire::Fire(float delta_time)
{
	if (condition_ == 0.f && true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(7.2))
	{
		CreateFire();
		condition_ = 1.f;
	}

	if (true == ptr_anim_ctrl_->CheckCurrentAnimationEnd(0.1))
	{
		fire_state_ = State::End;
		condition_ = 0.f;
	}
}

void CRioreus_Fire::CreateFire()
{
	const Matrix mat_fire_world = *ptr_target_->ptr_mat_fire() * ptr_transform_->mat_world();
	const Vector3 fire_pos = *(Vector3*)&mat_fire_world.m[3][0];
	Vector3 fire_dir = (*(Vector3*)&mat_fire_world.m[0][0]);
	fire_dir.y = 0.f;
	fire_dir = fire_dir.Normalize();

	TCHAR effect_key[64] = TEXT("");
	wsprintf(effect_key, TEXT("FireBall_Explosion_Effect"));
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CFireBall::Create(Engine::GraphicDevice()->GetDevice(), fire_pos, fire_dir));

}

void CRioreus_Fire::CreateExplosion()
{
	Sound()->StopSound(Sound()->CHANNEL_ENEMY_ATTACK_);
	Sound()->PlaySound(ptr_sound_explosion_, Sound()->CHANNEL_ENEMY_ATTACK_);

	Vector3 forward_vector = ptr_transform_->Forward().Normalize();
	Vector3 right_vector = ptr_transform_->Right().Normalize();

	Vector3 explosion_pos[3] = { 
		ptr_transform_->position() + forward_vector * 7.f
		, ptr_transform_->position() + forward_vector * 5.f + right_vector * 5.f
		, ptr_transform_->position() + forward_vector * 5.f + right_vector * -5.f };

	TCHAR effect_key[64] = TEXT("");
	for (int i = 0; i < 3; ++i)
	{
		wsprintf(effect_key, TEXT("Rioreus_Explosion_Effect_%d"), i);
		Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CRioreus_Explosion::Create(Engine::GraphicDevice()->GetDevice(), explosion_pos[i]));
	}
}
