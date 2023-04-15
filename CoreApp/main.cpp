#include "main.hpp"
#include "stdint.h"
#include "LowLevel.h"

#ifdef TARGET_POWERSTEP
volatile uint8_t TestArrayG431[32];
#endif

#ifdef TARGET_MOSFETS
volatile uint8_t TestArrayMosfets[32];
#endif

#ifdef TARGET_F429
volatile uint8_t TestArrayF429[32];
#endif

int main(void)
{
	LowLevel_Init();
}
