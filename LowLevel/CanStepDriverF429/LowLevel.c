#include "LowLevel.h"
#include "main.h"
// #include "fdcan.h"
#include "i2c.h"
// #include "spi.h"
#include "gpio.h"
#include "usart.h"
#include "stm32f4xx_it.h"

static const uint16_t I2C_EEPROM_TIMEOUT = 10U;
volatile bool unused_fix = false;

// this used for force linker to link interrupt handlers
void NotUsedFunc()
{
	NMI_Handler();
	HardFault_Handler();
	MemManage_Handler();
	BusFault_Handler();
	UsageFault_Handler();
	DebugMon_Handler();
	TIM1_UP_TIM10_IRQHandler();
	USART6_IRQHandler();
	HAL_UART_RxCpltCallback(NULL);
}

void LowLevel_Init()
{
	HAL_Init();
	SystemClock_Config();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

	MX_GPIO_Init();
	// MX_FDCAN1_Init();
	// MX_SPI1_Init();

	if (unused_fix)
	{
		NotUsedFunc();
	}
}

void LowLevel_Delay(uint32_t delay_ms)
{
	HAL_Delay(delay_ms);
}

void LowLevel_LedRGB_SetRed(uint8_t value)
{
	GPIO_PinState state = value == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, state);
}

void LowLevel_LedRGB_SetGreen(uint8_t value)
{
	GPIO_PinState state = value == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, state);
}

void LowLevel_LedRGB_SetBlue(uint8_t value)
{
	GPIO_PinState state = value == 0 ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, state);
}

void LowLevel_I2C_Init()
{
	MX_I2C2_Init();
}

LlErrorCode LowLevel_I2C_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *Data, uint16_t Size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, Size, I2C_EEPROM_TIMEOUT);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

LlErrorCode LowLevel_I2C_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *Data, uint16_t Size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, Data, Size, I2C_EEPROM_TIMEOUT);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

static LowLevel_RS485_ReceiveCallback RS485_Callback;

void LowLevel_RS485_Init(LowLevel_RS485_ReceiveCallback callback)
{
	RS485_Callback = callback;
	MX_USART6_UART_Init();
	HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
}

LlErrorCode LowLevel_RS485_Transmit(uint8_t *Data, uint16_t Size)
{
	HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart6, Data, Size, 100);
	HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

LlErrorCode LowLevel_RS485_Receive(uint8_t *Data, uint16_t Size)
{
	HAL_StatusTypeDef status = HAL_UART_Receive_IT(&huart6, Data, Size);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

// Table 9. 1 Mbyte dual bank Flash memory organization (STM32F42xxx and STM32F43xxx)
// 	Block Name Block base addresses Size

// Bank 1
// 	Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbytes
// 	Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbytes
// 	Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbytes
// 	Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbyte
// 	Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbytes
// 	Sector 5 0x0802 0000 - 0x0803 FFFF 128 Kbytes
// 	Sector 6 0x0804 0000 - 0x0805 FFFF 128 Kbytes
// 	- - -
// 	- - -
// 	- - -
// 	Sector 11 0x080E 0000 - 0x080F FFFF 128 Kbytes
// Bank 2
// 	Sector 12 0x0810 0000 - 0x0810 3FFF 16 Kbytes
// 	Sector 13 0x0810 4000 - 0x0810 7FFF 16 Kbytes
// 	Sector 14 0x0810 8000 - 0x0810 BFFF 16 Kbytes
// 	Sector 15 0x0810 C000 - 0x0810 FFFF 16 Kbytes
// 	Sector 16 0x0811 0000 - 0x0811 FFFF 64 Kbytes
// 	Sector 17 0x0812 0000 - 0x0813 FFFF 128 Kbytes
// 	Sector 18 0x0814 0000 - 0x0815 FFFF 128 Kbytes
// 	- -
// 	- -
// 	- -
// 	Sector 23 0x081E 0000 - 0x081F FFFF 128 Kbytes

// System memory 0x1FFF 0000 - 0x1FFF 77FF 30 Kbytes
// OTP 0x1FFF 7800 - 0x1FFF 7A0F 528 bytes
// Option bytes
// 	Bank 1 0x1FFF C000 - 0x1FFF C00F 16 bytes
// 	Bank 2 0x1FFE C000 - 0x1FFE C00F 16 bytes

uint32_t SectorAddr[24] = {
	0x08000000, 0x08004000, 0x08008000, 0x0800C000, 0x08010000, 0x08020000,
	0x08040000, 0x08060000, 0x08080000, 0x080A0000, 0x080C0000, 0x080E0000,
	0x08100000, 0x08104000, 0x08108000, 0x0810C000, 0x08110000, 0x08120000,
	0x08140000, 0x08160000, 0x08180000, 0x081A0000, 0x081C0000, 0x081E0000
};

uint32_t SectorSize[24] = {
	0x4000, 0x4000, 0x4000, 0x4000, 0x10000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000,
	0x4000, 0x4000, 0x4000, 0x4000, 0x10000, 0x20000,
	0x20000, 0x20000, 0x20000, 0x20000, 0x20000, 0x20000
};

uint32_t LowLevel_GetFreeFlashAddr()
{
	return SectorAddr[FLASH_SECTOR_12];
}

LlErrorCode LowLevel_WriteFlash(uint32_t Addr, uint8_t *Data, uint32_t Size)
{
	HAL_StatusTypeDef status = HAL_FLASH_Unlock();

	uint32_t SectorNum = 0xff;
	for (uint32_t i = 0; i < sizeof(SectorAddr); i++)
	{
		if (SectorAddr[i] == Addr)
		{
			SectorNum = i;
			break;
		}
	}

	if ((SectorNum == 0xff) || (status != HAL_OK))
	{
		HAL_FLASH_Lock();
		return LL_ERR;
	}

	uint32_t Sectors = 1;
	for (uint32_t i = SectorNum, SectorsSizeSumm = 0; i < sizeof(SectorSize); i++)
	{
		SectorsSizeSumm += SectorSize[i];

		if (SectorsSizeSumm >= Size)
			break;

		Sectors++;
	}

	FLASH_EraseInitTypeDef EraseStruct = {
		.TypeErase = FLASH_TYPEERASE_SECTORS,
		.Banks = Addr < SectorAddr[FLASH_SECTOR_12] ? FLASH_BANK_1 : FLASH_BANK_2,
		.VoltageRange = FLASH_VOLTAGE_RANGE_3,
		.Sector = SectorNum,
		.NbSectors = Sectors
	};
	uint32_t SectorError = 0;

	status = HAL_FLASHEx_Erase(&EraseStruct, &SectorError);
	if (status == HAL_OK)
	{
		size_t CurrWriteAddr = SectorAddr[SectorNum];
		uint8_t *CurrWritePtr = Data;
		while (Size >= sizeof(uint32_t))
		{
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, CurrWriteAddr, *((uint32_t*)CurrWritePtr));
			CurrWriteAddr += sizeof(uint32_t);
			CurrWritePtr += sizeof(uint32_t);
			Size -= sizeof(uint32_t);

			volatile uint32_t err = HAL_FLASH_GetError();
		}

		while (Size > 0)
		{
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, CurrWriteAddr, *CurrWritePtr);
			CurrWriteAddr += sizeof(uint8_t);
			CurrWritePtr += sizeof(uint8_t);
			Size -= sizeof(uint8_t);
		}
	}

	HAL_FLASH_Lock();

	return status == HAL_OK ? LL_OK : LL_ERR;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart6)
	{
		RS485_Callback();
	}
}
