#pragma once

typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR4 Vector4;
typedef D3DXMATRIX Matrix;

struct Vector3 : public D3DXVECTOR3
{
	Vector3()
	{
		x = 0.f; y = 0.f; z = 0.f;
	}
	Vector3(float value)
	{
		x = value; y = value; z = value;
	}
	Vector3(float _x, float _y)
	{
		x = _x; y = _y; z = 0.f;
	}
	Vector3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}
	Vector3(const Vector3& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
	}
	Vector3(Vector3&& rhs)
	{
		*this = std::move(rhs);
	}
	Vector3& operator=(const Vector3& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
		return *this;
	}
	


	// Vector3 + float
	Vector3 operator+(float value)
	{
		const float _x = x + value;
		const float _y = y + value;
		const float _z = z + value;

		return Vector3(_x, _y, _z);
	}
	Vector3& operator+=(float value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}

	// Vector + Vector
	Vector3 operator+(const Vector3& dst)
	{
		const float _x = x + dst.x;
		const float _y = y + dst.y;
		const float _z = z + dst.z;

		return Vector3(_x, _y, _z);
	}
	Vector3& operator+=(const Vector3& dst)
	{
		x += dst.x;
		y += dst.y;
		z += dst.z;

		return *this;
	}

	// Vector3 - float
	Vector3 operator-(float value)
	{
		const float _x = x - value;
		const float _y = y - value;
		const float _z = z - value;

		return Vector3(_x, _y, _z);
	}
	Vector3& operator-=(float value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}

	// Vector - Vector
	Vector3 operator-(const Vector3& dst)
	{
		const float _x = x - dst.x;
		const float _y = y - dst.y;
		const float _z = z - dst.z;

		return Vector3(_x, _y, _z);
	}
	Vector3& operator-=(const Vector3& dst)
	{
		x -= dst.x;
		y -= dst.y;
		z -= dst.z;

		return *this;
	}

	// Vector3 * float
	Vector3 operator*(float value)
	{
		const float _x = x * value;
		const float _y = y * value;
		const float _z = z * value;

		return Vector3(_x, _y, _z);
	}
	Vector3& operator*=(float value)
	{
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}

	// Vector3 / float
	Vector3 operator/(float value)
	{
		const float _x = x / value;
		const float _y = y / value;
		const float _z = z / value;

		return Vector3(_x, _y, _z);
	}
	Vector3& operator/=(float value)
	{
		x /= value;
		y /= value;
		z /= value;

		return *this;
	}
	bool operator==(const Vector3& rhs)
	{
		//float result = fabs((x - rhs.x) + (y - rhs.y) + (z - rhs.z));
		//return result < 0.001f;

		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const Vector3& rhs)
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}

	// Function
	float Magnitude()
	{
		return (x * x) + (y * y) + (z * z);
	}
	float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}
	float Distance(Vector3& dst)
	{
		return fabsf((*this - dst).Length());
	}
	const Vector3 Normalize()
	{
		const float length = this->Length();
		
		return Vector3(x / length, y / length, z / length);
	}

	// Dot Product
	static float Dot(const Vector3& src, const Vector3& dst)
	{
		return (src.x * dst.x) + (src.y * dst.y) + (src.z * dst.z);
	}

	// Cross Product
	static Vector3 Cross(const Vector3& src, const Vector3& dst)
	{
		const float _x = (src.y * dst.z) - (src.z * dst.y);
		const float _y = (src.z * dst.x) - (src.x * dst.z);
		const float _z = (src.x * dst.y) - (src.y * dst.x);

		return Vector3(_x, _y, _z);
	}

	static Vector3 Right()
	{
		return Vector3(1.f, 0.f, 0.f);
	}

	static Vector3 Up()
	{
		return Vector3(0.f, 1.f, 0.f);
	}

	static Vector3 Forward()
	{
		return Vector3(0.f, 0.f, 1.f);
	}
};