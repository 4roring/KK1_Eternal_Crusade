#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader
	: public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CShader();

public:
	const LPD3DXEFFECT GetEffectHandle();
	virtual CComponent* CloneComponent() override;

public:
	HRESULT AddShaderFile(const TCHAR* file_path);
	void BegineShader(int pass_index);
	void EndShader();

public:
	virtual int Release();

public:
	static CShader* Create(LPDIRECT3DDEVICE9 ptr_device, const TCHAR* file_path);

private:
	LPDIRECT3DDEVICE9 ptr_device_ = nullptr;
	LPD3DXEFFECT ptr_effect_ = nullptr;
	LPD3DXBUFFER ptr_error_message_ = nullptr;
};

END