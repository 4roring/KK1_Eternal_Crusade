#pragma once

#include "Engine_Include.h"

BEGIN(Engine)

class CNavLine
{
private:
	CNavLine();

public:
	~CNavLine();

public:
	HRESULT InitNavLine(const Vector3& start_point, const Vector3& end_point);
	bool CheckLine(const Vector2& move_point);

public:
	static CNavLine* Create(const Vector3& start_point, const Vector3& end_point);

public:
	Vector3 SlidingDirection(const Vector3& in_vector);

private:
	Vector2 start_point_ = Vector2(0.f, 0.f);
	Vector2 end_point_ = Vector2(0.f, 0.f);
	Vector2 normal_ = Vector2(0.f, 0.f);
};

END