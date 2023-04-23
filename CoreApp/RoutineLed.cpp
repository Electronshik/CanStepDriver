#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "Globals.hpp"

using namespace Board::LedRGB;

void Routines::Led::Routine(void *pvParameters)
{
	GlCmd cmd;
	for(;;)
	{
		cmd.Type = GlCmdType::NO;

		if (!GlCmdQueue.empty())
		{
			GlCmdQueue.pop_into (cmd);
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

		OS::Delay(30);
	}
}
