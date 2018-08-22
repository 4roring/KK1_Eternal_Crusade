#include "stdafx.h"
#include "Granade.h"

#include "Transform.h"
#include "Shader.h"
#include "StaticMesh.h"
#include "Bomb.h"
#include "GranadeTrail.h"

CGranade::CGranade(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CGranade::~CGranade()
{
	Release();
}

HRESULT CGranade::Initialize(const Vector3& start_pos, const Vector3& start_dir)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	color_[0] = Vector4(0.2f, 0.2f, 0.2f, 1.f);
	color_[1] = Vector4(0.45f, 0.45f, 0.45f, 1.f);
	color_[2] = Vector4(0.1f, 0.1f, 0.1f, 1.f);

	ptr_transform_->position() = start_pos;
	pos_y_ = ptr_transform_->position().y;
	ptr_transform_->rotation().z = 1.57f;
	ptr_transform_->move_dir() = start_dir;

	ptr_transform_->scale() = Vector3(0.01f, 0.01f, 0.01f);

	velocity_ = Vector3(0.f, 3.f, 30.f);
	Engine::CGameObject::Update(0.f);

	ptr_bomb_sound_ = Sound()->FindSound(TEXT("Granade_Bomb"));

	ptr_trail_ = CGranadeTrail::Create(ptr_device_, &ptr_transform_->mat_world());
	Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, TEXT("Granade_Trail"), ptr_trail_);

	return S_OK;
}

void CGranade::Update(float delta_time)
{
	if (current_cell_index_ == -1)
		current_cell_index_ = Engine::GameManager()->FindCellIndex(ptr_transform_->position());

	Engine::CGameObject::Update(delta_time);
	// 포물선, 시간지난 후 폭파
	destroy_time_ += delta_time;

	velocity_.y -= 10.f * delta_time;
	velocity_.z = max(velocity_.z - 5.f * delta_time, 0.f);

	pos_y_ += velocity_.y * delta_time;
	Vector3 move_dir = ptr_transform_->move_dir() * velocity_.z * delta_time;
	
	Vector2 out_line_normal = Vector2(0.f, 0.f);
	int option = -1;
	if(current_cell_index_ != -1)
		current_cell_index_ = Engine::GameManager()->MoveFromNavMesh(ptr_transform_->position(), move_dir, current_cell_index_, option, out_line_normal);
	if (option != -1)
	{
		// 반사!
		const Vector2 src_dir = Vector2(move_dir.x, move_dir.z) * -1.f;
		const Vector2 dst_dir = 2 * (out_line_normal * D3DXVec2Dot(&src_dir, &out_line_normal));
		move_dir.x += dst_dir.x;
		move_dir.z += dst_dir.y;
		ptr_transform_->move_dir() = move_dir.Normalize();
		velocity_.y -= 0.5f;
		velocity_.z = max(velocity_.z - 10.f, 0.f);
	}

	if (pos_y_ > ptr_transform_->position().y)
		ptr_transform_->position().y = pos_y_;
	else
		velocity_.z = max(velocity_.z - 100.f * delta_time, 0.f);

	if (pos_y_ > 0.f)
		ptr_transform_->rotation().y += 12.f * delta_time;
}

void CGranade::LateUpdate()
{
	if (destroy_time_ >= 3.f)
	{
		TCHAR effect_key[64] = TEXT("");
		wsprintf(effect_key, TEXT("Bomb_Effect"));
		Engine::GameManager()->GetCurrentScene()->AddObject(MAINTAIN_STAGE, effect_key, CBomb::Create(ptr_device_, ptr_transform_->position()));
		destroy_ = true;
		Sound()->PlaySound(ptr_bomb_sound_, Sound()->CHANNEL_PLAYER_ATTACK);
	}
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
}

void CGranade::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	if (nullptr == ptr_effect) return;

	Matrix mat_view, mat_proj;
	ptr_device_->GetTransform(D3DTS_VIEW, &mat_view);
	ptr_device_->GetTransform(D3DTS_PROJECTION, &mat_proj);

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view);
	ptr_effect->SetMatrix("g_mat_projection", &mat_proj);

	ptr_effect->SetVector("set_color_r", &color_[0]);
	ptr_effect->SetVector("set_color_g", &color_[1]);
	ptr_effect->SetVector("set_color_b", &color_[2]);

	ptr_mesh_->RenderMesh(ptr_effect, 1);
}

CGranade * CGranade::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& start_pos, const Vector3& start_dir)
{
	CGranade* ptr_obj = new CGranade(ptr_device);
	if (FAILED(ptr_obj->Initialize(start_pos, start_dir)))
	{
		Safe_Delete(ptr_obj);
		assert(!"Ork_Gun Create Failed");
	}
	return ptr_obj;

}

HRESULT CGranade::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_NormalMap"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of LevelObject Shader Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Granade_Mesh"), TEXT("Mesh"), ptr_mesh_);
	assert(hr == S_OK && "Mesh ReadyComponent Failed");

	return hr;
}

void CGranade::Release()
{
	if(ptr_trail_ != nullptr)
		ptr_trail_->Destroy();
}
