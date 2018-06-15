#include "CubeTexture.h"

Engine::CCubeTexture::CCubeTexture(LPDIRECT3DDEVICE9 ptr_device)
	: CVIBuffer(ptr_device)
{
}

Engine::CCubeTexture::~CCubeTexture()
{
}

HRESULT Engine::CCubeTexture::InitBuffer()
{
	vertex_size_ = sizeof(CubeTex);
	vertex_count_ = 8;
	vertex_FVF_ = CubeTex_FVF;

	index_size_ = sizeof(Index16);
	index_format_ = D3DFMT_INDEX16;
	tri_count_ = 12;

	HRESULT hr = CVIBuffer::CreateBuffer();
	assert(hr == S_OK && "Create Buffer Error");

	CubeTex* ptr_vertex = nullptr;
	ptr_vertex_buffer_->Lock(0, 0, (void**)&ptr_vertex, 0);

	ptr_vertex[0].position = Vector3(-1.f, 1.f, -1.f);
	ptr_vertex[1].position = Vector3(1.f, 1.f, -1.f);
	ptr_vertex[2].position = Vector3(1.f, -1.f, -1.f);
	ptr_vertex[3].position = Vector3(-1.f, -1.f, -1.f);

	ptr_vertex[4].position = Vector3(-1.f, 1.f, 1.f);
	ptr_vertex[5].position = Vector3(1.f, 1.f, 1.f);
	ptr_vertex[6].position = Vector3(1.f, -1.f, 1.f);
	ptr_vertex[7].position = Vector3(-1.f, -1.f, 1.f);

	for (int i = 0; i < 8; ++i)
		ptr_vertex[i].texture_uv = ptr_vertex[i].position;

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

Engine::CComponent * Engine::CCubeTexture::CloneComponent()
{
	++reference_count_;
	return this;
}

Engine::CCubeTexture * Engine::CCubeTexture::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CCubeTexture* ptr_buffer = new CCubeTexture(ptr_device);
	if (FAILED(ptr_buffer->InitBuffer()))
	{
		Safe_Delete(ptr_buffer);
		assert(!"CubeTexture Buffer Create Failed");
	}
	ptr_buffer->AddReferenceCount();
	return ptr_buffer;
}