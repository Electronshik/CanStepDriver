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

}
