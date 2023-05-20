#include "BoardInterface.hpp"
#include "LowLevel.h"

using namespace Board;

RS485::RS485()
{
	LowLevel_RS485_Init(RS485::ReceiveCallback);
}

ErrorCode RS485::Transmit(etl::array<uint8_t, TransmitBuffSize> Data)
{
	LlErrorCode Error = LlErrorCode::LL_ERR;

	if (this->TransmitMutex.try_lock())
	{
		LlErrorCode Error = LowLevel_RS485_Transmit((uint8_t*)Data.data(), (uint16_t)Data.size());
		this->TransmitMutex.unlock();
	}

	return Error == LlErrorCode::LL_OK ? ErrorCode::OK : ErrorCode::ERR;
}

ErrorCode RS485::Transmit(uint8_t *Data, uint16_t Size)
{
	LlErrorCode Error = LlErrorCode::LL_ERR;

	if (this->TransmitMutex.try_lock())
	{
		LlErrorCode Error = LowLevel_RS485_Transmit(Data, Size);
		this->TransmitMutex.unlock();
	}

	return Error == LlErrorCode::LL_OK ? ErrorCode::OK : ErrorCode::ERR;
}

void RS485::Receive()
{
	LowLevel_RS485_Receive(this->LLReceiveBuffer, this->ReceiveSize);
}

void RS485::Receive(uint8_t ReceiveSize)
{
	if (ReceiveSize <= ReceiveBuffSize)
		this->ReceiveSize = ReceiveSize;

	LowLevel_RS485_Receive(this->LLReceiveBuffer, this->ReceiveSize);
}

void RS485::ReceiveCallback()
{
	RS485 *RS485Instance = RS485::GetInstance();
	if (RS485Instance->ReceiveBuffMutex.try_lock())
	{
		if ((RS485Instance->ReceiveBuffer.size() + RS485Instance->ReceiveSize) > ReceiveBuffSize)
		{
			RS485Instance->ReceiveBuffer.clear();
		}

		for (size_t i = 0; i < RS485Instance->ReceiveSize; i++)
		{
			RS485Instance->ReceiveBuffer.push_back(RS485Instance->LLReceiveBuffer[i]);
		}
		RS485Instance->ReceiveBuffMutex.unlock();
	}

	RS485Instance->Receive();
}
