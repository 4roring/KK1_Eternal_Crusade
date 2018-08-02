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
	CComponent* GetComponent(const std::wstring& component_key) const;
	void AddComponent(const std::wstring& instance_key, CComponent* ptr_component);

public:
	void SetActive(bool active);
	void Destroy();
	void set_is_render(bool is_render);

public:
	void ComputeViewZ(const Matrix& mat_view);
	float view_z() const;
	bool active() const;
	bool destroy() const;
	Engine::CTransform* transform();

public:
	virtual ~CGameObject();
	
protected:
	HRESULT Initialize();

public:
	virtual void LateInit();
	virtual void Update(float delta_time);
	virtual void LateUpdate();
	virtual void Render();

public:
	virtual void ApplyDamage(int damage);
	virtual bool CheckRaycast(const Vector3& ray_pos, const Vector3& ray_dir, float* hit_dist);

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
#ifdef _DEBUG
	CShader* ptr_debug_shader_ = nullptr;
#endif


protected:
	float view_z_ = 0.f;
	bool active_ = true;
	bool destroy_ = false;
	bool is_render_ = false;

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
	if (nullptr == ptr_object)
		return E_FAIL;

	AddComponent(instance_key, ptr_component);
	return S_OK;
}

END


