#pragma once
#include "etl/mutex.h"
#include "etl/vector.h"
#include "main.hpp"

namespace Board
{
	void Init();

	namespace LedRGB
	{
		void SetRed(uint8_t value);
		void SetGreen(uint8_t value);
		void SetBlue(uint8_t value);
	} // namespace LedRGB

	namespace Flash
	{
		uint32_t GetFreeAddr();
		void Write(uint32_t Addr, uint8_t* Data, uint32_t Size);
	} // namespace Flash
} // namespace Board
