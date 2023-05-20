#include "SharedLib.hpp"
#include "LowLevel.h"

uint32_t glob;

void Flash_Led()
{
	glob++;

	LowLevel_LedRGB_SetGreen(0);
	while (1)
	{
		LowLevel_LedRGB_SetBlue(30);
		LowLevel_LedRGB_SetRed(0);
		uint32_t cnt = 500000;
		while (cnt--)
		{
			/* code */
		}

		LowLevel_LedRGB_SetRed(30);
		LowLevel_LedRGB_SetBlue(0);

		cnt = 500000;
		while (cnt--)
		{
			/* code */
		}
	}
}

void Flash_NoLed()
{
	glob--;
}

//arm-none-eabi-objdump -CDz --source ./build/libShared.so
