#include "LowLevel.h"
#include "main.h"
#include "fdcan.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

void LowLevel_Init(void)
{
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_FDCAN1_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
}
