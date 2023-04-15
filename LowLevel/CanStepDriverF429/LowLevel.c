#include "LowLevel.h"
#include "main.h"
// #include "fdcan.h"
#include "i2c.h"
// #include "spi.h"
#include "gpio.h"
#include "stm32f4xx_it.h"

static const uint16_t I2C_EEPROM_TIMEOUT = 10U;

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
}

void LowLevel_Init()
{
	HAL_Init();
	SystemClock_Config();
	// HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	// HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	// HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

	MX_GPIO_Init();
	// MX_FDCAN1_Init();
	MX_I2C2_Init();
	// MX_SPI1_Init();
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

void LowLevel_I2C_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	HAL_I2C_Mem_Read(&hi2c2, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, I2C_EEPROM_TIMEOUT);
}

void LowLevel_I2C_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	HAL_I2C_Mem_Write(&hi2c2, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, I2C_EEPROM_TIMEOUT);
}
