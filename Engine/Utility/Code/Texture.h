#pragma once

#include "Resources.h"

BEGIN(Engine)

struct VecTexture
{
	std::vector<IDirect3DBaseTexture9*> vec_texture;
};

class ENGINE_DLL CTexture
	: public CResources
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CTexture();

public:
	const LPDIRECT3DBASETEXTURE9 GetTexture(int index);

public:
	virtual CComponent* CloneComponent() override;

public:
	HRESULT LoadTexture(TEXTURETYPE texture_type, const std::wstring& file_path, int count);
	
public:
	virtual int Release() override;

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 ptr_device
		, TEXTURETYPE texture_type
		, const std::wstring& file_path
		, int count);

private:
	VecTexture texture_;

private:
	int container_size = 0;
};

END