#include "main.hpp"
#include "BoardMemory.hpp"
#include "BoardInterface.hpp"

namespace Board
{
	using MemoryType = Memory_I2C;
	extern MemoryParams_t MemoryParams;

	using FRAM_MemoryType = Memory_FRAM;
	extern MemoryParams_t FRAM_MemoryParams;

	using InterfaceType = RS485;
}
