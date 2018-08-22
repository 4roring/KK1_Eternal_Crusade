#include "TrailColor.h"

Engine::CTrailColor::CTrailColor(LPDIRECT3DDEVICE9 ptr_device)
	: ptr_device_(ptr_device)
{
}

Engine::CTrailColor::~CTrailColor()
{

}

Engine::CComponent * Engine::CTrailColor::CloneComponent()
{
	++reference_count_;
	return this;
}

HRESULT Engine::CTrailColor::Initialize()
{
	vertex_size_ = sizeof(VertexColor);
	vertex_FVF_ = VertexColor_FVF;
	++reference_count_;
	return S_OK;
}

void Engine::CTrailColor::SetTrail(const Vector3 * pos, const Vector3 * dir, float length, const D3DXCOLOR& color, DWORD count)
{
	ptr_vertex_ = new VertexColor[count];
	ZeroMemory(ptr_vertex_, vertex_size_ * count);

	DWORD half_count = DWORD(count * 0.5f);

	for (DWORD i = 0; i < half_count; ++i)
	{
		Vector3 position = pos[i];
		Vector3 direction = dir[i];

		float _length = length * (i * 2.f / half_count);

		ptr_vertex_[i * 2].position = position - direction * _length;
		ptr_vertex_[i * 2].color = D3DXCOLOR(color.r, color.g, color.b, i / (float)half_count);
		ptr_vertex_[i * 2 + 1].position = position + direction * _length;
		ptr_vertex_[i * 2 + 1].color = D3DXCOLOR(color.r, color.g, color.b, i / (float)half_count);
	}
	poly_ = count - 2;
}

void Engine::CTrailColor::Render()
{
	ptr_device_->SetFVF(vertex_FVF_);
	ptr_device_->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, poly_, ptr_vertex_, vertex_size_);
	Safe_Delete_Array(ptr_vertex_);
}

int Engine::CTrailColor::Release()
{
	if (--reference_count_ == 0)
	{
		if (ptr_vertex_ != nullptr)
			Safe_Delete_Array(ptr_vertex_);
	}
	return reference_count_;
}

Engine::CTrailColor * Engine::CTrailColor::Create(LPDIRECT3DDEVICE9 ptr_device)
{
	CTrailColor* ptr_buffer = new CTrailColor(ptr_device);
	ptr_buffer->Initialize();

	return ptr_buffer;
}
