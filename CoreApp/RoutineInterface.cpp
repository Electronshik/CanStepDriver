#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "Globals.hpp"
#include <cstdio>
#include "etl/string.h"

using namespace Board::RS485;

void Routines::Interface::Routine(void *pvParameters)
{
	uint8_t nl = '\n';
	Init();
	Transmit((uint8_t*)GenSettings.str, strlen(GenSettings.str));
	Transmit(&nl, 1);

	Receive();

	etl::string<MaxReceiveSize> ReceivedString;

	for(;;)
	{
		if (!ReceiveBuffer.empty())
		{
			OS::Delay(1);
			ReceivedString.clear();

			if (ReceiveBuffMutex.try_lock())
			{
				ReceivedString.assign(ReceiveBuffer.begin(), ReceiveBuffer.end());
				ReceiveBuffer.clear();
				ReceiveBuffMutex.unlock();
				printf("%s", ReceivedString.c_str());
			}

			GlCmd cmd = {.Type = GlCmdType::NO};
			if (ReceivedString.compare("1") == 0)
			{
				cmd.Type = GlCmdType::LED;
				cmd.Value = 1;
			}
			else if (ReceivedString.compare("2") == 0)
			{
				cmd.Type = GlCmdType::LED;
				cmd.Value = 2;
			}
			else if (ReceivedString.compare("3") == 0)
			{
				cmd.Type = GlCmdType::LED;
				cmd.Value = 3;
			}

			if (cmd.Type != GlCmdType::NO)
			{
				GlCmdQueue.emplace(cmd);
			}
		}

		OS::Delay(10);
	}
}
