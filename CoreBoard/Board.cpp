#include "Board.hpp"
#include "LowLevel.h"

static uint8_t RS485_prvReceiveBuffer[Board::RS485::MaxReceiveSize];

namespace Board
{

	void Init()
	{
		LowLevel_Init();

		LowLevel_LedRGB_SetRed(0);
		LowLevel_LedRGB_SetGreen(0);
		LowLevel_LedRGB_SetBlue(0);
	}

	namespace LedRGB
	{
		void SetRed(uint8_t value)
		{
			LowLevel_LedRGB_SetRed(value);
		}

		void SetGreen(uint8_t value)
		{
			LowLevel_LedRGB_SetGreen(value);
		}

		void SetBlue(uint8_t value)
		{
			LowLevel_LedRGB_SetBlue(value);
		}
	}

	namespace RS485
	{
		uint8_t ReceiveSize = 1;
		etl::vector<uint8_t, MaxReceiveSize> ReceiveBuffer;
		etl::mutex ReceiveBuffMutex;

		void ReceiveCallback()
		{
			if (ReceiveBuffMutex.try_lock())
			{
				if ((ReceiveBuffer.size() + ReceiveSize) > MaxReceiveSize)
				{
					ReceiveBuffer.clear();
				}

				for (size_t i = 0; i < ReceiveSize; i++)
				{
					ReceiveBuffer.push_back(RS485_prvReceiveBuffer[i]);
				}
				ReceiveBuffMutex.unlock();
			}

			Receive();
		}

		void Init()
		{
			LowLevel_RS485_Init(ReceiveCallback);
		}

		void Transmit(uint8_t *pData, uint16_t Size)
		{
			LowLevel_RS485_Transmit(pData, Size);
		}

		void Receive()
		{
			LowLevel_RS485_Receive(RS485_prvReceiveBuffer, ReceiveSize);
		}
	}
}
