#include "BoardConfig.hpp"

namespace Board
{

	MemoryParams_t MemoryParams =
	{
		.BeginAddr = 0x1000,
		.FullSize = 32768,
		.ErasedVal = 0xff,
		.PageSize = 64,
		.BusAddr = 0x50
	};

	MemoryParams_t FRAM_MemoryParams =
	{
		.BeginAddr = 0x00,
		.FullSize = 8192,
		.ErasedVal = 0x00,
		.PageSize = 0,
		.BusAddr = 0x57
	};

}
