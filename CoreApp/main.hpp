#pragma once
#include <cstddef>
#include <cstdint>
#include "StorableSettings.hpp"

enum class ErrorCode
{
	OK,
	ERR
};

extern GeneralSetting GenSettings;
extern MotorSetting MotorSettings;

template<class S>
class Singleton
{
	public:
		static S* GetInstance()	//noexcept(std::is_nothrow_constructible<T>::value)
		{
			if(Instance == nullptr)
				Instance = new S();
				
			return Instance;
		}

	protected:
		static inline S* Instance = nullptr;
		Singleton() noexcept = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		virtual ~Singleton() = default; //-Weffc++
};
