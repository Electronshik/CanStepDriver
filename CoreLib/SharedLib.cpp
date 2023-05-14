#include "SharedLib.hpp"

uint32_t glob;

void Flash_Led()
{
	glob++;
}

void Flash_NoLed()
{
	glob--;
}

//arm-none-eabi-objdump -CDz --source ./build/libShared.so
