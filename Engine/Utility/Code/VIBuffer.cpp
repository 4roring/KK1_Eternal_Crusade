#include "VIBuffer.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 ptr_device)
	: CResources(ptr_device)
{
}

Engine::CVIBuffer::~CVIBuffer()
{
}

void Engine::CVIBuffer::GetVertexInfo(void * ptr_vertex)
{
	void* ptr_original_vertex = nullptr;
	ptr_vertex_buffer_->Lock(0, 0, &ptr_original_vertex, 0);
	memcpy(ptr_vertex, ptr_original_vertex, vertex_size_ * vertex_count_);
	ptr_vertex_buffer_->Unlock();
}

void Engine::CVIBuffer::SetVertexInfo(void * ptr_vertex)
{
	void* ptr_original_vertex = nullptr;
	ptr_vertex_buffer_->Lock(0, 0, &ptr_original_vertex, 0);
	memcpy(ptr_original_vertex, ptr_vertex, vertex_size_ * vertex_count_);
	ptr_vertex_buffer_->Unlock();
}

void Engine::CVIBuffer::SetIndexInfo(void * ptr_index, const DWORD * ptr_tri_count)
{
	void* ptr_original_index = nullptr;
	tri_count_ = *ptr_tri_count;

	ptr_index_buffer_->Lock(0, 0, &ptr_original_index, 0);
	memcpy(ptr_original_index, ptr_index, index_size_ * tri_count_);
	ptr_index_buffer_->Unlock();
}

void Engine::CVIBuffer::Render()
{
	ptr_device_->SetStreamSource(0, ptr_vertex_buffer_, 0, vertex_size_);
	ptr_device_->SetFVF(vertex_FVF_);
	ptr_device_->SetIndices(ptr_index_buffer_);
	ptr_device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vertex_count_, 0, tri_count_);
}

int Engine::CVIBuffer::Release()
{
	if (--reference_count_ == 0)
	{
		Safe_Release(ptr_vertex_buffer_);
		Safe_Release(ptr_index_buffer_);
	}

	return reference_count_;
}

HRESULT Engine::CVIBuffer::CreateBuffer()
{
	HRESULT hr = E_FAIL;
	hr = ptr_device_->CreateVertexBuffer(vertex_size_ * vertex_count_
		, 0
		, vertex_FVF_
		, D3DPOOL_MANAGED, &ptr_vertex_buffer_, nullptr);
	assert(hr == S_OK && "Create Vertex Buffer Error");

	if (index_size_ == 0)
		return S_OK;

	hr = ptr_device_->CreateIndexBuffer(index_size_ * tri_count_
		, 0
		, index_format_
		, D3DPOOL_MANAGED, &ptr_index_buffer_, nullptr);
	assert(hr == S_OK && "Create Index Buffer Error");

	return S_OK;
}
