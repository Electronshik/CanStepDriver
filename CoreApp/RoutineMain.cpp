#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "BoardConfig.hpp"
#include "MemoryStorage.hpp"

Memory<Board::MemoryType> MemoryStorage(Board::MemoryParams);

GeneralSetting GenSettings(0);
MotorSetting MotorSettings(64);

void Routines::Main::Routine(void *pvParameters)
{
	MemoryStorage.RestoreSetting(&GenSettings);
	MemoryStorage.RestoreSetting(&MotorSettings);

	for(;;)
	{
		OS::Delay(100);
	}
}
