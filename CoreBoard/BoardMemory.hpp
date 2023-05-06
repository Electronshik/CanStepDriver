#pragma once
#include "main.hpp"

namespace Board
{
	typedef struct
	{
		uint32_t BeginAddr;
		uint32_t FullSize;
		uint32_t ErasedVal;
		uint32_t PageSize;
		uint32_t BusAddr;
	} MemoryParams_t;

	class IMemory
	{
		public:
			MemoryParams_t &MemoryParams;
			virtual void Erase(size_t MemAddress, uint32_t Size) = 0;
			virtual void Write(size_t MemAddress, uint8_t *pData, uint32_t Size) = 0;
			virtual void Read(size_t MemAddress, uint8_t *pData, uint32_t Size) = 0;
			virtual ~IMemory() {}

		protected:
			IMemory(MemoryParams_t &MemoryParams) : MemoryParams(MemoryParams) {}
	};

	class Memory_I2C : public IMemory
	{
		public:
			Memory_I2C(MemoryParams_t &MemoryParams);
			void Erase(size_t MemAddress, uint32_t Size) override;
			void Write(size_t MemAddress, uint8_t *pData, uint32_t Size) override;
			void Read(size_t MemAddress, uint8_t *pData, uint32_t Size) override;
			~Memory_I2C() {};

		private:
			const uint32_t WriteDelay = 10;
	};
}
