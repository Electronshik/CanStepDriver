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

LlErrorCode LowLevel_I2C_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c2, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, I2C_EEPROM_TIMEOUT);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

LlErrorCode LowLevel_I2C_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c2, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, Size, I2C_EEPROM_TIMEOUT);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

static LowLevel_RS485_ReceiveCallback RS485_Callback;

void LowLevel_RS485_Init(LowLevel_RS485_ReceiveCallback callback)
{
	RS485_Callback = callback;
	MX_USART6_UART_Init();
	HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
}

LlErrorCode LowLevel_RS485_Transmit(uint8_t *pData, uint16_t Size)
{
	HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart6, pData, Size, 100);
	HAL_GPIO_WritePin (RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);
	return status == HAL_OK ? LL_OK : LL_ERR;
}

LlErrorCode LowLevel_RS485_Receive(uint8_t *pData, uint16_t Size)
{
	HAL_UART_Receive_IT(&huart6, pData, Size);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart6)
	{
		RS485_Callback();
	}
}
