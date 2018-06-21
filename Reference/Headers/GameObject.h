#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;
class CTransform;
class CShader;
class CComponentMap;

class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 ptr_device);

private:
	void Release();

public:
	const CComponent* GetComponent(const std::wstring& component_key);
	void AddComponent(const std::wstring& instance_key, CComponent* ptr_component);

public:
	void SetActive(bool active);
	void Destroy();

public:
	void ComputeViewZ(const Matrix& mat_view);
	float view_z() const;
	bool active() const;
	bool destroy() const;
	Engine::CTransform* transform();

public:
	virtual ~CGameObject();
	
protected:
	virtual HRESULT Initialize();

public:
	virtual void Update(float delta_time);
	virtual void Render();

protected:
	template<typename T>
	HRESULT Ready_Component(const int container_index
		, const std::wstring& component_key
		, const std::wstring& instance_key
		, T& ptr_object);

protected:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	CTransform* ptr_transform_ = nullptr;
	CShader* ptr_shader_ = nullptr;

protected:
	float view_z_ = 0.f;
	bool active_ = true;
	bool destroy_ = false;

private:
	CComponentMap* ptr_component_map_ = nullptr;
};

template<typename T>
inline HRESULT CGameObject::Ready_Component(const int container_index
	, const std::wstring & component_key
	, const std::wstring & instance_key
	, T & ptr_object)
{
	CComponent* ptr_component = nullptr;
	ptr_component = Engine::GameManager()->CloneComponent(container_index, component_key);
	ptr_object = dynamic_cast<T>(ptr_component);
	assert(nullptr != ptr_object && "Ready_Component dynamic_cast Error");
	AddComponent(instance_key, ptr_component);
	return S_OK;
}

END


