#include "Board.hpp"
#include "LowLevel.h"

void Board_Init()
{
	LowLevel_Init();

	LowLevel_LedRGB_SetRed(0);
	LowLevel_LedRGB_SetGreen(0);
	LowLevel_LedRGB_SetBlue(0);
	while (1)
	{
		LowLevel_LedRGB_SetRed(10);
		LowLevel_Delay(300);
		LowLevel_LedRGB_SetRed(0);
		LowLevel_Delay(300);
	}
}
