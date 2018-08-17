#include "stdafx.h"
#include "UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 ptr_device)
	: Engine::CGameObject(ptr_device)
{
	D3DXMatrixIdentity(&mat_view_);
}

CUI::~CUI()
{
}

void CUI::Update(float delta_time)
{
	CGameObject::Update(delta_time);

	mat_view_._11 = width_;
	mat_view_._22 = height_;
	
	mat_view_._41 = point_.x - (kWinCx >> 1);
	mat_view_._42 = -point_.y + (kWinCy >> 1);
}
