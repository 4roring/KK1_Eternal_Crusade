#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CSwordEletro
	: public Engine::CGameObject
{
private:
	explicit CSwordEletro(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CSwordEletro();

private:
	HRESULT Initialize(Matrix* ptr_parent);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CSwordEletro* Create(LPDIRECT3DDEVICE9 ptr_device, Matrix* ptr_parent);

private:
	HRESULT AddComponent();
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	Matrix* ptr_parent_ = nullptr;
};