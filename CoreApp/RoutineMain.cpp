#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "BoardConfig.hpp"
#include "MemoryStorage.hpp"
#include "MotorController.hpp"

GeneralSetting GenSettings(0);
MotorSetting MotorSettings(64);

void Routines::Main::Routine(void *pvParameters)
{
	Memory<>* MemoryStorage = Memory<>::GetInstance();
	MemoryStorage->RestoreSetting(&GenSettings);
	MemoryStorage->RestoreSetting(&MotorSettings);

	MotorController<MotorVirtual>* Motor = new(MotorController<MotorVirtual>);

	for(;;)
	{
		OS::Delay(100);
	}
}
