#pragma once

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
}