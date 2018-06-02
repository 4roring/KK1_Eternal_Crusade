#pragma once

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer
	: public CResources
{
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 ptr_device);

public:
	virtual ~CVIBuffer();

public:
	void GetVertexInfo(void* ptr_vertex);

public:
	void SetVertexInfo(void* ptr_vertex);
	void SetIndexInfo(void* ptr_index, const DWORD* ptr_tri_count);

public:
	virtual void Render();
	virtual int Release() override;

public:
	HRESULT CreateBuffer();

protected:
	LPDIRECT3DVERTEXBUFFER9 ptr_vertex_buffer_ = nullptr;
	DWORD vertex_size_ = 0;
	DWORD vertex_count_ = 0;
	DWORD vertex_FVF_ = 0;

protected:
	LPDIRECT3DINDEXBUFFER9 ptr_index_buffer_ = nullptr;
	DWORD index_size_ = 0;
	DWORD tri_count_ = 0;
	D3DFORMAT index_format_ = D3DFMT_UNKNOWN;
};

END