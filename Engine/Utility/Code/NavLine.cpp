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
	normal_ = Vector2(dir.y * -1.f, dir.x);
	D3DXVec2Normalize(&normal_, &normal_);

	return S_OK;
}

bool Engine::CNavLine::CheckLine(const Vector2 & move_point)
{
	const Vector2 dir = move_point - start_point_;
	float dot_result = D3DXVec2Dot(&dir, &normal_);

	if (dot_result > 0.f) return true;
	else return false;
}

Vector2 Engine::CNavLine::normal()
{
	return normal_;
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

Vector3 Engine::CNavLine::SlidingDirection(const Vector3 & in_dir)
{
	Vector2 out_dir = { in_dir.x, in_dir.z };
	Vector2 temp_normal = Vector2(0.f, 0.f);
	D3DXVec2Normalize(&temp_normal, &normal_);
	temp_normal *= D3DXVec2Dot(&-out_dir, &normal_);
	out_dir += temp_normal * 1.1f;

	return Vector3(out_dir.x, in_dir.y, out_dir.y);
}