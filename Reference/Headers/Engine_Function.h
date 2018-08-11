#pragma once

#include <random>

namespace Engine
{
	template <typename T> 
	void Safe_Delete(T& pointer)
	{
		if (nullptr != pointer)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& pointer_array)
	{
		if (nullptr != pointer_array)
		{
			delete[] pointer_array;
			pointer_array = nullptr;
		}
	}

	template <typename T>
	int Safe_Release(T& pointer)
	{
		int reference_count = 0;
		if (nullptr != pointer)
		{
			reference_count = pointer->Release();
			if (reference_count == 0)
				pointer = nullptr;
		}
		return reference_count;
	}

	template <typename T>
	int Safe_Release_Delete(T& pointer)
	{
		int reference_count = 0;
		if (nullptr != pointer)
		{
			reference_count = pointer->Release();
			if (reference_count == 0)
			{
				delete pointer;
				pointer = nullptr;
			}
		}
		return reference_count;
	}

	static float RandomFloat(float _min, float _max)
	{
		if (_min >= _max)
			return _min;

		float rand_num = (rand() % 10000) * 0.0001f;
		return (rand_num * (_max - _min)) + _min;
	}

	static Vector3 GetRandomVector(const Vector3& _min, const Vector3& _max)
	{
		return{ RandomFloat(_min.x, _max.x), RandomFloat(_min.y, _max.y), RandomFloat(_min.z, _max.z) };
	}

	/* RandomFloat 업그레이드 방안.
	int interger_min =

	std::random_device _random_device;
	std::mt19937 mt(_random_device());
	std::uniform_int_distribution<int> range(_min, _max);

	return range(mt);*/
}