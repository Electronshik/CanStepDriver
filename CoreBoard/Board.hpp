#pragma once
#include "main.hpp"
#include "etl/vector.h"
#include "etl/mutex.h"

namespace Board
{
	void Init();

	namespace LedRGB
	{
		void SetRed(uint8_t value);
		void SetGreen(uint8_t value);
		void SetBlue(uint8_t value);
	}

	namespace RS485
	{
		inline constexpr uint8_t MaxReceiveSize = 64;
		extern etl::vector<uint8_t, MaxReceiveSize> ReceiveBuffer;
		extern etl::mutex ReceiveBuffMutex;
		void Init();
		void Transmit(uint8_t *pData, uint16_t Size);
		void Receive();
	}
}
