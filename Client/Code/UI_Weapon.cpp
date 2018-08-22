#include "stdafx.h"
#include "UI_Weapon.h"

#include "Transform.h"
#include "Shader.h"
#include "RectTexture.h"
#include "Texture.h"
#include "SpaceMarinObserver.h"
#include "Font.h"

CUI_Weapon::CUI_Weapon(LPDIRECT3DDEVICE9 ptr_device)
	: CUI(ptr_device)
{
}

CUI_Weapon::~CUI_Weapon()
{
	Release();
}

HRESULT CUI_Weapon::Initialize()
{
	HRESULT hr = Engine::CGameObject::Initialize();
	assert(!FAILED(hr) && "UI_Weapon Initialize Falied");
	hr = AddComponent();
	assert(!FAILED(hr) && "UI_Weapon AddComponent Falied");

	Subject()->RegisterObserver(ptr_player_observer_);

	ptr_transform_->position().x = 1.f;

	point_ = Vector2(950.f, 600.f);
	width_ = 395.f * 0.15f;
	height_ = 265.f * 0.15f;

	return S_OK;
}

void CUI_Weapon::LateInit()
{
	Subject()->Notify();
}

void CUI_Weapon::Update(float delta_time)
{
	CUI::Update(delta_time);

	bullet_count_ = max(ptr_player_observer_->GetBullet(0), 0);
	granade_count_ = max(ptr_player_observer_->GetGranade(0), 0);
}

void CUI_Weapon::LateUpdate()
{
	Engine::GameManager()->AddRenderLayer(Engine::LAYER_UI, this);
}

void CUI_Weapon::Render()
{
	LPD3DXEFFECT ptr_effect = ptr_shader_->GetEffectHandle();
	ptr_effect->SetMatrix("g_mat_world", &ptr_transform_->mat_world());
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetMatrix("g_mat_projection", &Subject()->GetOrthoProjection());
	ptr_effect->SetTexture("g_color_texture", ptr_gun_texture_->GetTexture(0));
	
	ptr_effect->Begin(nullptr, 0);
	
	// Gun Image
	ptr_effect->BeginPass(0);
	ptr_buffer_->Render();
	ptr_effect->EndPass();

	mat_view_._11 = 100.f * 0.7f;
	mat_view_._22 = 52.f * 0.7f;
	mat_view_._41 = 1100.f - (kWinCx >> 1);
	mat_view_._42 = -700.f + (kWinCy >> 1);
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetTexture("g_color_texture", ptr_pannel_texture_->GetTexture(0));
	ptr_effect->CommitChanges();

	// Pannel
	ptr_effect->BeginPass(0);
	ptr_buffer_->Render();
	ptr_effect->EndPass();

	mat_view_._11 = 36.f;
	mat_view_._22 = 36.f;
	mat_view_._41 = 1100.f - (kWinCx >> 1);
	mat_view_._42 = -700.f + (kWinCy >> 1);
	ptr_effect->SetMatrix("g_mat_view", &mat_view_);
	ptr_effect->SetTexture("g_color_texture", ptr_granade_texture_->GetTexture(0));
	ptr_effect->CommitChanges();

	// Granade
	ptr_effect->BeginPass(0);
	ptr_buffer_->Render();
	ptr_effect->EndPass();

	ptr_effect->End();

	TCHAR text[128] = TEXT("");
	wsprintf(text, TEXT("%d/¡Ä"), bullet_count_);
	ptr_font_->Render(text, D3DXCOLOR(1.f, 0.41f, 0.01f, 1.f), Vector3(1080.f, 580.f));

	wsprintf(text, TEXT("%d"), granade_count_);
	ptr_font_->Render(text, D3DXCOLOR(1.f, 0.41f, 0.01f, 1.f), Vector3(1180.f, 670.f));
}

CUI_Weapon * CUI_Weapon::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CUI_Weapon* ptr_ui = new CUI_Weapon(ptr_device);
	if (FAILED(ptr_ui->Initialize()))
	{
		Safe_Delete(ptr_ui);
		assert(!"UI_Hp Create Failed");
	}

	return ptr_ui;
}

HRESULT CUI_Weapon::AddComponent()
{
	HRESULT hr = E_FAIL;

	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Component_Transform"), TEXT("Transform"), ptr_transform_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Weapon Tranform Component ");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Shader_UI"), TEXT("Shader"), ptr_shader_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Weapon Shader Component");
	hr = Ready_Component(MAINTAIN_STATIC, TEXT("Buffer_RectTexture"), TEXT("Buffer"), ptr_buffer_);
	assert(hr == S_OK && "ReadyComponent Failed of CUI_Weapon Buffer Component");

	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Panel_UI_Texture"), TEXT("Texture_Pannel"), ptr_pannel_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Weapon Texture Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Granade_UI_Texture"), TEXT("Texture_Granade"), ptr_granade_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Weapon Texture Component");
	hr = Ready_Component(MAINTAIN_STAGE, TEXT("Gun_UI_Texture"), TEXT("Texture_Gun"), ptr_gun_texture_);
	assert(!FAILED(hr) && "ReadyComponent Failed of CUI_Weapon Texture Component");

	ptr_font_ = Engine::GraphicDevice()->GetFont(TEXT("FFFFORWA"));

	return S_OK;
}

void CUI_Weapon::Release()
{
}
