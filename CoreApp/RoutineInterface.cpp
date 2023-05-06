#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "BoardConfig.hpp"
#include "BoardInterface.hpp"
#include "Globals.hpp"
#include <cstdio>
#include "etl/string.h"

using namespace Board;

void Routines::Interface::Routine(void *pvParameters)
{
	OS::Delay(100);

	uint8_t nl = '\n';
	IInterface *IFace = InterfaceType::GetInstance();
	IFace->Transmit((uint8_t*)GenSettings.str, strlen(GenSettings.str));
	IFace->Transmit(&nl, 1);

	IFace->Transmit((uint8_t*)"Test string\n", 12);
	IFace->Receive();

	etl::string<IFace->ReceiveBuffSize> ReceivedString;

	for(;;)
	{
		if (!IFace->ReceiveBuffer.empty())
		{
			OS::Delay(1);
			ReceivedString.clear();

			if (IFace->ReceiveBuffMutex.try_lock())
			{
				ReceivedString.assign(IFace->ReceiveBuffer.begin(), IFace->ReceiveBuffer.end());
				IFace->ReceiveBuffer.clear();
				IFace->ReceiveBuffMutex.unlock();
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
