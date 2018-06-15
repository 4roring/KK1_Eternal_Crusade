#include "CubeColor.h"

Engine::CCubeColor::CCubeColor(LPDIRECT3DDEVICE9 ptr_device)
	: CVIBuffer(ptr_device)
{
}

Engine::CCubeColor::~CCubeColor()
{
}

Engine::CComponent * Engine::CCubeColor::CloneComponent()
{
	++reference_count_;
	return this;
}

void Engine::CCubeColor::SetBoundingBox(const Vector3 & min, const Vector3 & max, DWORD color)
{
	VertexColor* ptr_vertex = nullptr;
	ptr_vertex_buffer_->Lock(0, 0, (void**)&ptr_vertex, 0);

	ptr_vertex[0].position = Vector3(min.x, max.x, min.z);
	ptr_vertex[1].position = Vector3(max.x, max.x, min.z);
	ptr_vertex[2].position = Vector3(max.x, min.z, min.z);
	ptr_vertex[3].position = Vector3(min.z, min.z, min.z);

	ptr_vertex[4].position = Vector3(min.z, max.x, max.x);
	ptr_vertex[5].position = Vector3(max.x, max.x, max.x);
	ptr_vertex[6].position = Vector3(max.x, min.z, max.x);
	ptr_vertex[7].position = Vector3(min.z, min.z, max.x);

	for (int i = 0; i < 8; ++i)
		ptr_vertex[i].color = color;

	ptr_vertex_buffer_->Unlock();
}

HRESULT Engine::CCubeColor::CreateBuffer(const Vector3 & min, const Vector3 & max, DWORD color)
{
	vertex_size_ = sizeof(VertexColor);
	vertex_count_ = 8;
	vertex_FVF_ = VertexColor_FVF;

	index_size_ = sizeof(Index16);
	index_format_ = D3DFMT_INDEX16;
	tri_count_ = 12;

	HRESULT hr = CVIBuffer::CreateBuffer();
	assert(hr == S_OK && "Create Buffer Error");

	VertexColor* ptr_vertex = nullptr;
	ptr_vertex_buffer_->Lock(0, 0, (void**)&ptr_vertex, 0);

	ptr_vertex[0].position = Vector3(min.x, max.x, min.z);
	ptr_vertex[1].position = Vector3(max.x, max.x, min.z);
	ptr_vertex[2].position = Vector3(max.x, min.z, min.z);
	ptr_vertex[3].position = Vector3(min.z, min.z, min.z);

	ptr_vertex[4].position = Vector3(min.z, max.x, max.x);
	ptr_vertex[5].position = Vector3(max.x, max.x, max.x);
	ptr_vertex[6].position = Vector3(max.x, min.z, max.x);
	ptr_vertex[7].position = Vector3(min.z, min.z, max.x);

	for (int i = 0; i < 8; ++i)
		ptr_vertex[i].color = color;

	ptr_vertex_buffer_->Unlock();

	Index16* ptr_index = nullptr;
	ptr_index_buffer_->Lock(0, 0, (void**)&ptr_index, 0);

	//+x
	ptr_index[0]._1 = 1;	ptr_index[0]._2 = 5;	ptr_index[0]._3 = 6;
	ptr_index[1]._1 = 1;	ptr_index[1]._2 = 6;	ptr_index[1]._3 = 2;

	//-x
	ptr_index[2]._1 = 4;	ptr_index[2]._2 = 0;	ptr_index[2]._3 = 3;
	ptr_index[3]._1 = 4;	ptr_index[3]._2 = 3;	ptr_index[3]._3 = 7;

	//+y
	ptr_index[4]._1 = 4;	ptr_index[4]._2 = 5;	ptr_index[4]._3 = 1;
	ptr_index[5]._1 = 4;	ptr_index[5]._2 = 1;	ptr_index[5]._3 = 0;

	//-y
	ptr_index[6]._1 = 3;	ptr_index[6]._2 = 2;	ptr_index[6]._3 = 6;
	ptr_index[7]._1 = 3;	ptr_index[7]._2 = 6;	ptr_index[7]._3 = 7;

	//+z
	ptr_index[8]._1 = 7;	ptr_index[8]._2 = 6;	ptr_index[8]._3 = 5;
	ptr_index[9]._1 = 7;	ptr_index[9]._2 = 5;	ptr_index[9]._3 = 4;

	//-z
	ptr_index[10]._1 = 0;	ptr_index[10]._2 = 1;	ptr_index[10]._3 = 2;
	ptr_index[11]._1 = 0;	ptr_index[11]._2 = 2;	ptr_index[11]._3 = 3;

	ptr_index_buffer_->Unlock();
	return S_OK;
}

Engine::CCubeColor * Engine::CCubeColor::Create(LPDIRECT3DDEVICE9 ptr_device, const Vector3 & min, const Vector3 & max, DWORD color)
{
	CCubeColor* ptr_buffer = new CCubeColor(ptr_device);
	if (FAILED(ptr_buffer->CreateBuffer(min, max, color)))
	{
		Safe_Delete(ptr_buffer);
		assert(!"CubeColor Create Failed");
	}
	ptr_buffer->AddReferenceCount();
	return ptr_buffer;
}
