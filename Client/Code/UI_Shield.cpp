#include "stdafx.h"
#include "UI_Shield.h"

#include "Transform.h"
#include "Shader.h"
#include "RectTexture.h"
#include "Texture.h"
#include "SpaceMarinObserver.h"

CUI_Shield::CUI_Shield(LPDIRECT3DDEVICE9 ptr_device)
	: CUI(ptr_device)
{
}

CUI_Shield::~CUI_Shield()
{
	Release();
}

HRESULT CUI_Shield::Initialize()
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "Explosion Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "Explosion AddComponent Falied");

	Subject()->RegisterObserver(ptr_player_observer_);

	ptr_transform_->position().x = 1.f;

	point_ = Vector2(100.f, 675.f);
	width_ = 292.f * 0.5f;
	height_ = 64.f * 0.5f;

	return S_OK;

}

void CUI_Shield::LateInit()
{
	Subject()->Notify();
}

void CUI_Shield::Update(float delta_time)
{
	CUI::Update(delta_time);
	if(max_shield_ == 0) max_shield_ = ptr_player_observer_->GetShiled(0);

	shield_ = max(ptr_player_observer_->GetShiled(0), 0);
}

void CUI_Shield::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_UI, this);
}

void CUI_Shield::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();

	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetMatrix("g_mat_projection", &Subject()->GetOrthoProjection());
	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(0));

	ptr_effect->Begin(nullptr, 0);

	ptr_effect->BeginPass(0);
	ptr_buffer_->Render();
	ptr_effect->EndPass();

	const float hp_ratio = (float)shield_ / max_shield_;
	mat_view_._11 *= hp_ratio;
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetFloat("g_max_u", hp_ratio);
	ptr_effect->SetTexture("g_color_texture", ptr_texture_->GetTexture(1));
	ptr_effect->CommitChanges();

	ptr_effect->BeginPass(1);
	ptr_buffer_->Render();
	ptr_effect->EndPass();

	ptr_effect->End();
}

CUI_Shield * CUI_Shield::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CUI_Shield* ptr_ui = new CUI_Shield(ptr_device);
	if (FAILED(ptr_ui->Initialize()))
	{
		Safe_Delete(ptr_ui);
		assert(!"UI_Hp Create Failed");
	}

	return ptr_ui;
}

HRESULT CUI_Shield::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of UI_Hp Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_UI"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of UI_Hp Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "ReadyComponent Failed of UI_Hp Buffer Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Shield_UI_Texture"), TEXT("Texture"), ptr_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of UI_Hp Texture Component");

	return S_OK;
}

void CUI_Shield::Release()
{
	Subject()->RemoveObserver(ptr_player_observer_);
}
