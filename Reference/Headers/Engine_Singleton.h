#pragma once

namespace Engine
{
	template<typename T>
	class CSingleton
	{
	protected:
		CSingleton() {}
		virtual ~CSingleton() {}
		CSingleton(const CSingleton&) = delete;
		CSingleton& operator=(CSingleton&) = delete;

	public:
		static T* GetInstance()
		{
			if (ptr_instance_ == nullptr)
				ptr_instance_ = new T;
			return ptr_instance_;
		}

		static void DestroyInstance()
		{
			if (nullptr != ptr_instance_)
			{
				delete ptr_instance_;
				ptr_instance_ = nullptr;
			}
		}

	private:
		static T* ptr_instance_;
	};

	template<typename T> T* CSingleton<T>::ptr_instance_ = nullptr;
}
