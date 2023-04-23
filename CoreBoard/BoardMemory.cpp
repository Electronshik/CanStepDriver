#include "BoardMemory.hpp"
#include "LowLevel.h"

using namespace Board;

Memory_I2C::Memory_I2C(MemoryParams_t &MemoryParams) : IMemory(MemoryParams)
{
	LowLevel_I2C_Init();
}

void Memory_I2C::Erase(size_t MemAddress, uint32_t Size)
{

}

void Memory_I2C::Write(size_t MemAddress, uint8_t *pData, uint32_t Size)
{
	uint8_t Data[this->MemoryParams.PageSize];
	uint16_t i = 0;

	while (i < Size)
	{
		uint16_t j = 0;
		while ((j < this->MemoryParams.PageSize) && (i < Size))
		{
			Data[j] = pData[i];
			i++;
			j++;
		}

		// if ( HAL_OK != HAL_I2C_Mem_Write(&hi2c2, I2C_EEPROM_ADDRESS << 1, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, j, I2C_EEPROM_TIMEOUT) )
		// {
		// 	return -1;
		// }
		LowLevel_I2C_Write((uint16_t)(this->MemoryParams.BusAddr << 1), (uint16_t)MemAddress, Data, j);
		MemAddress += this->MemoryParams.PageSize;
		LowLevel_Delay(this->WriteDelay);
	}
	// return 1;
}

void Memory_I2C::Read(size_t MemAddress, uint8_t *pData, uint32_t Size)
{
	uint8_t Data[this->MemoryParams.PageSize];
	uint16_t i = 0;

	while (i < Size)
	{
		uint16_t j = this->MemoryParams.PageSize;
		// if ( HAL_OK != HAL_I2C_Mem_Read (&hi2c2, I2C_EEPROM_ADDRESS << 1, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, j, I2C_EEPROM_TIMEOUT))
		// {
		// 	return -1;
		// }

		LowLevel_I2C_Read((uint16_t)(this->MemoryParams.BusAddr << 1), (uint16_t)MemAddress, Data, j);
		j = 0;
		while((j < this->MemoryParams.PageSize) && (i < Size))
		{
			pData[i] = Data[j];
			i++;
			j++;
		}
		MemAddress += this->MemoryParams.PageSize;
		LowLevel_Delay(this->WriteDelay);
	}
	// return 1;
}
