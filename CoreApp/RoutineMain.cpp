#include "Routines.hpp"
#include "OS_Wrappers.hpp"
#include "Board.hpp"
#include "BoardConfig.hpp"
#include "MemoryStorage.hpp"
#include "DBStorage.hpp"
#include "MotorController.hpp"
#include "stdio.h"
#include <etl/string.h>
#include <etl/array.h>
#include <etl/crc.h>
// #include <algorithm>

GeneralSetting GenSettings(0);
MotorSetting MotorSettings(64);

void Routines::Main::Routine(void *pvParameters)
{
	Memory<>* MemoryStorage = Memory<>::GetInstance();
	MemoryStorage->RestoreSetting(&GenSettings);
	MemoryStorage->RestoreSetting(&MotorSettings);

	DBStorage<>* DBStor = DBStorage<>::GetInstance();

	char str[] = "Hello!\r\n";
	DBStor->SetBuff((uint8_t*)str, 8);
	// DBStor->SetBuff((uint8_t*)"1234", 4);

	char readed[32];
	DBStor->GetBuff((uint8_t*)readed, 8);
	readed[8] = '\0';
	printf("Readed: %s\n", readed);
	etl::string<32> toHash;
	toHash.assign(readed);
	etl::crc32 crc_calc;
	etl::array<unsigned char, 32> data { 9, 1, 8, 2, 7, 3, 6, 4, 5 };
	etl::copy(data.begin(), data.end(), crc_calc.input());

	printf("crc_calc: 0x%x\n", crc_calc.value());

	MotorController<MotorVirtual>* Motor = new(MotorController<MotorVirtual>);

	for(;;)
	{
		Motor->GoTo(100);
		OS::Delay(100);
	}
}
