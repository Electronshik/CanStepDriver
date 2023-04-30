#include "Board.hpp"
#include "LowLevel.h"

namespace Board
{

	void Init()
	{
		LowLevel_Init();

		LowLevel_LedRGB_SetRed(0);
		LowLevel_LedRGB_SetGreen(0);
		LowLevel_LedRGB_SetBlue(0);
	}

	namespace LedRGB
	{
		void SetRed(uint8_t value)
		{
			LowLevel_LedRGB_SetRed(value);
		}

		void SetGreen(uint8_t value)
		{
			LowLevel_LedRGB_SetGreen(value);
		}

		void SetBlue(uint8_t value)
		{
			LowLevel_LedRGB_SetBlue(value);
		}
	}
}
