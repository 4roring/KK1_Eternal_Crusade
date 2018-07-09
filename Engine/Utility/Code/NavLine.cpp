#include "NavLine.h"

Engine::CNavLine::CNavLine()
{
}

Engine::CNavLine::~CNavLine()
{
}

HRESULT Engine::CNavLine::InitNavLine(const Vector3 & start_point, const Vector3 & end_point)
{
	start_point_ = Vector2(start_point.x, start_point.z);
	end_point_ = Vector2(end_point.x, end_point.z);

	Vector2 dir = end_point_ - start_point_;
	normal_ = Vector2(-dir.y, dir.x);
	D3DXVec2Normalize(&normal_, &normal_);

	return S_OK;
}

Engine::CNavLine * Engine::CNavLine::Create(const Vector3 & start_point, const Vector3 & end_point)
{
	CNavLine* ptr_navline = new CNavLine;
	if (FAILED(ptr_navline->InitNavLine(start_point, end_point)))
	{
		Safe_Delete(ptr_navline);
		assert(!"NavLine Create Failed");
	}

	return ptr_navline;
}
