#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

typedef enum
{
	LL_OK,
	LL_ERR
} LlErrorCode;

void LowLevel_Init();
void LowLevel_Delay(uint32_t delay_ms);

// for gpio leds any value above 0 flashes led, for pwm leds the value sets pwm duty cycle
void LowLevel_LedRGB_SetRed(uint8_t value);
void LowLevel_LedRGB_SetGreen(uint8_t value);
void LowLevel_LedRGB_SetBlue(uint8_t value);

void LowLevel_I2C_Init();
LlErrorCode LowLevel_I2C_Read(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);
LlErrorCode LowLevel_I2C_Write(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t Size);

typedef void(*LowLevel_RS485_ReceiveCallback)();
void LowLevel_RS485_Init(LowLevel_RS485_ReceiveCallback callback);
LlErrorCode LowLevel_RS485_Transmit(uint8_t *pData, uint16_t Size);
LlErrorCode LowLevel_RS485_Receive(uint8_t *pData, uint16_t Size);

#ifdef __cplusplus
}
#endif
