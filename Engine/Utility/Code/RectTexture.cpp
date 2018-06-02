#include "RectTexture.h"

Engine::CRectTexture::CRectTexture(LPDIRECT3DDEVICE9 ptr_device)
	: CVIBuffer(ptr_device)
{
}

Engine::CRectTexture::~CRectTexture()
{
}

Engine::CComponent * Engine::CRectTexture::CloneComponent()
{
	++reference_count_;
	return this;
}

Engine::CRectTexture * Engine::CRectTexture::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CRectTexture* ptr_buffer = new CRectTexture(ptr_device);
	if (FAILED(ptr_buffer->InitBuffer()))
	{
		Safe_Delete(ptr_buffer);
		assert(!"Create RectTexture Failed");
	}
	ptr_buffer->AddReferenceCount();
	return ptr_buffer;
}

HRESULT Engine::CRectTexture::InitBuffer()
{
	vertex_size_ = sizeof(VertexTex);
	vertex_count_ = 4;
	vertex_FVF_ = VertexTex_FVF;

	index_size_ = sizeof(Index16);
	index_format_ = D3DFMT_INDEX16;
	tri_count_ = 2;

	HRESULT hr = CVIBuffer::CreateBuffer();
	assert(hr == S_OK && "Buffer Create Error");

	VertexTex* ptr_vertex = nullptr;
	ptr_vertex_buffer_->Lock(0, 0, (void**)&ptr_vertex, 0);

	ptr_vertex[0].position = Vector3(-1.f, 1.f, 0.f);
	ptr_vertex[0].normal = Vector3(0.f, 0.f, -1.f);
	ptr_vertex[0].texture_uv = Vector2(0.f, 0.f);

	ptr_vertex[1].position = Vector3(1.f, 1.f, 0.f);
	ptr_vertex[1].normal = Vector3(0.f, 0.f, -1.f);
	ptr_vertex[1].texture_uv = Vector2(1.f, 0.f);

	ptr_vertex[2].position = Vector3(1.f, -1.f, 0.f);
	ptr_vertex[2].normal = Vector3(0.f, 0.f, -1.f);
	ptr_vertex[2].texture_uv = Vector2(1.f, 1.f);

	ptr_vertex[3].position = Vector3(-1.f, -1.f, 0.f);
	ptr_vertex[3].normal = Vector3(0.f, 0.f, -1.f);
	ptr_vertex[3].texture_uv = Vector2(0.f, 1.f);

	ptr_vertex_buffer_->Unlock();

	Index16* ptr_index = nullptr;
	ptr_index_buffer_->Lock(0, 0, (void**)&ptr_index, 0);

	ptr_index[0]._1 = 0;
	ptr_index[0]._2 = 1;
	ptr_index[0]._3 = 2;

	ptr_index[1]._1 = 0;
	ptr_index[1]._2 = 2;
	ptr_index[1]._3 = 3;

	ptr_index_buffer_->Unlock();

	return S_OK;
}
