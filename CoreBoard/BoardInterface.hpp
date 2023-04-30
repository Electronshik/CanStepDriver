#pragma once
#include "main.hpp"
#include <etl/array.h>
#include "etl/vector.h"
#include "etl/mutex.h"

namespace Board
{
	class IInterface
	{
		public:
			static const uint16_t ReceiveBuffSize = 64;
			static const uint16_t TransmitBuffSize = 64;
			etl::vector<uint8_t, ReceiveBuffSize> ReceiveBuffer;
			etl::mutex ReceiveBuffMutex;
			virtual ErrorCode Transmit(etl::array<uint8_t, 64> Data) = 0;
			virtual ErrorCode Transmit(uint8_t *Data, uint16_t Size) = 0;
			virtual void Receive() = 0;

		protected:
			IInterface() {};
			// virtual ~IInterface() {};
	};

	class RS485 : public IInterface, public Singleton<RS485>
	{
		friend class Singleton<RS485>;

		public:
			static void ReceiveCallback();

			uint8_t ReceiveSize = 1;
			uint8_t LLReceiveBuffer[ReceiveBuffSize];
			ErrorCode Transmit(etl::array<uint8_t, 64> Data) override;
			ErrorCode Transmit(uint8_t *Data, uint16_t Size) override;
			void Receive() override;
			void Receive(uint8_t ReceiveSize);

		private:
			RS485();
			RS485(const RS485&) = delete;
			RS485& operator=(RS485&) = delete;
			~RS485() = default;
	};
}
