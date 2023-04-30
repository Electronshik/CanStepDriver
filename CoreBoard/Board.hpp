#pragma once
#include "main.hpp"
#include "etl/vector.h"
#include "etl/mutex.h"

namespace Board
{
	void Init();

	namespace LedRGB
	{
		void SetRed(uint8_t value);
		void SetGreen(uint8_t value);
		void SetBlue(uint8_t value);
	}
}
