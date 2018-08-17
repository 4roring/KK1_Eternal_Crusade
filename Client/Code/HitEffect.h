#pragma once

#include "GameObject.h"

BEGIN(Engine)
class CParticleSystem;
END

class CHitEffect
	: public Engine::CGameObject
{
private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CHitEffect();

private:
	HRESULT Initialize(const Vector3& position, const std::wstring& effect_key);

public:
	virtual void Update(float delta_time) override;
	virtual void LateUpdate() override;
	virtual void Render() override;

public:
	static CHitEffect* Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3& position, const std::wstring& effect_key);

private:
	HRESULT AddComponent(const std::wstring& effect_key);
	void Release();

private:
	Engine::CParticleSystem* ptr_particle_ = nullptr;
	
private:
	Engine::CShader* ptr_effect_shader_ = nullptr;
	Engine::CParticleSystem* ptr_blood_particle_ = nullptr;

private:
	float life_time = 0.f;

};