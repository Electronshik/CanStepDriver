#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "Globals.hpp"
#include <cstdio>

using namespace Board::LedRGB;

uint8_t testArr[128];

void Routines::Led::Routine(void *pvParameters)
{
	GlCmd cmd;
	SetBlue(10);

	for(;;)
	{
		cmd.Type = GlCmdType::NO;

		if (!GlCmdQueue.empty())
		{
			GlCmdQueue.pop_into(cmd);
		}

		if (cmd.Type == GlCmdType::LED)
		{
			SetRed(0);
			SetGreen(0);
			SetBlue(0);
			switch (cmd.Value)
			{
				case 1:
					SetRed(10);
					break;

				case 2:
					SetGreen(10);
					break;

				case 3:
					SetBlue(10);
					break;

				default:
					break;
			}
		}
		else if (cmd.Type == GlCmdType::FLASH)
		{
			uint32_t Addr = Board::Flash::GetFreeAddr();
			for (size_t i = 0; i < sizeof(testArr); i++)
				testArr[i] = i;

			Board::Flash::Write(Addr, testArr, sizeof(testArr));

			uint32_t CmpFaults = 0;
			uint8_t *WrittenPtr = (uint8_t*)Addr;
			for (size_t i = 0; i < sizeof(testArr); i++)
			{
				if (testArr[i] != WrittenPtr[i])
					CmpFaults++;
			}

			printf("Flash Cmp Faults: %d\r\n", CmpFaults);
		}

		OS::Delay(30);
	}
}
