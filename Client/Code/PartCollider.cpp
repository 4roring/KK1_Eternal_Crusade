#include "stdafx.h"
#include "PartCollider.h"

#include "Collider.h"
#include "Shader.h"
#include "Transform.h"

CPartCollider::CPartCollider(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
}

CPartCollider::~CPartCollider()
{
	Release();
}

void CPartCollider::SetAttackState(bool attack_state)
{
	attack_state_ = attack_state;
}

void CPartCollider::SetPartMatrix(const Matrix * ptr_mat_part)
{
	ptr_mat_part_ = ptr_mat_part;
}

HRESULT CPartCollider::Initialize(const Matrix * mat_part, CRioreus::PartColl part, CRioreus* target)
{
	HRESULT hr = CGameObject::Initialize();
	assert(!FAILED(hr) && "CGameObject::Initialize call failed in SpaceMarin");

	hr = AddComponent();
	assert(!FAILED(hr) && "AddComponent call failed in SpaceMarin");

	ptr_mat_part_ = mat_part;
	ptr_target_ = target;
	part_ = part;
	return S_OK;
}

void CPartCollider::Update(float delta_time)
{
	CGameObject::Update(delta_time);
	ptr_transform_->mat_world() = ptr_target_->transform()->mat_world();

	if (damage_time_ != 0.f)
		damage_time_ = max(damage_time_ - delta_time, 0.f);

	switch (part_)
	{
	case CRioreus::Part_Head:
		ptr_collider_->SetSphereCollider(0.7f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	case CRioreus::Part_Body:
		ptr_collider_->SetSphereCollider(1.5f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	case CRioreus::Part_Left_Leg:
		ptr_collider_->SetSphereCollider(0.8f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	case CRioreus::Part_Right_Leg:
		ptr_collider_->SetSphereCollider(0.8f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	case CRioreus::Part_Left_Wing:
		ptr_collider_->SetSphereCollider(1.0f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	case CRioreus::Part_Right_Wing:
		ptr_collider_->SetSphereCollider(1.0f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	case  CRioreus::Part_Tail:
		ptr_collider_->SetSphereCollider(1.2f, *(Vector3*)&ptr_mat_part_->m[3][0]);
		break;
	}
	CollSystem()->AddColliderList(ptr_collider_, TAG_ENEMY);
	CollSystem()->AddRaycastList(ptr_collider_, TAG_ENEMY);
}

void CPartCollider::LateUpdate()
{
#ifdef _DEBUG
	Engine::GameManager()->AddRenderLayer(Engine::RENDERLAYER::LAYER_NONEALPHA, this);
#endif // _DEBUG

	if (attack_state_ == true)
	{
		const CollList& space_marin_coll_list = CollSystem()->GetColliderList(TAG_PLAYER);
		for (auto& space_marin_coll : space_marin_coll_list)
		{
			if (true == ptr_collider_->TriggerCheck(space_marin_coll))
				space_marin_coll->ptr_object()->ApplyDamage(100);
		}
	}
}

void CPartCollider::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	Matrix mat_coll;
	D3DXMatrixIdentity(&mat_coll);
	mat_coll._41 = ptr_collider_->GetSpherePos().x;
	mat_coll._42 = ptr_collider_->GetSpherePos().y;
	mat_coll._43 = ptr_collider_->GetSpherePos().z;
	ptr_effect->SetMatrix("g_mat_world", &mat_coll);

	ptr_shader_->BegineShader(1);
	ptr_collider_->DebugRender();
	ptr_shader_->EndShader();
}

void CPartCollider::ApplyDamage(int damage)
{
	if (damage == 100 && damage_time_ != 0.f) return;
	else if (damage == 100) damage_time_ = 0.7f;

	switch (part_)
	{
	case CRioreus::Part_Head:
		damage =  int(damage * 1.3f);
		++ptr_target_->head_condition();
		break;
	case CRioreus::Part_Body:
		damage = int(damage * 0.9f);
		break;
	case  CRioreus::Part_Left_Leg:
		damage = int(damage * 0.6f);
		++ptr_target_->left_leg_condition();
		break;
	case  CRioreus::Part_Right_Leg:
		damage = int(damage * 0.6f);
		++ptr_target_->right_leg_condition();
		break;
	case  CRioreus::Part_Left_Wing:
		damage = int(damage * 0.5f);
		break;
	case  CRioreus::Part_Right_Wing:
		damage = int(damage * 0.5f);
		break;
	case  CRioreus::Part_Tail:
		damage = int(damage * 0.4f);
		++ptr_target_->tail_condition();
		break;
	}
	ptr_target_->ApplyDamage(damage);
}

HRESULT CPartCollider::AddComponent()
{
	HRESULT hr = E_FAIL;
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(hr == S_OK && "Tranform Component ReadyComponent Failed");
#ifdef _DEBUG
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_Default"), TEXT("Debug_Shader"), ptr_shader_);
	assert(hr == S_OK && "Ork Shader_Default ReadyComponent Failed");
#endif

	ptr_collider_ = Engine::CCollider::Create(ptr_device_, this, Collider_Sphere);

	return S_OK;
}

void CPartCollider::Release()
{
	Engine::Safe_Release_Delete(ptr_collider_);
}

CPartCollider * CPartCollider::Create(LPDIRECT3DDEVICE9 ptr_device, const Matrix * mat_part, CRioreus::PartColl part, CRioreus * target)
{
	CPartCollider* ptr_coll = new CPartCollider(ptr_device);
	if (FAILED(ptr_coll->Initialize(mat_part, part, target)))
	{
		Safe_Delete(ptr_coll);
		assert(!"PartCollider Craete Failed");
	}

	return ptr_coll;
}
