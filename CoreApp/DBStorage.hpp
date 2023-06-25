#pragma once
#include "main.hpp"
#include "BoardMemory.hpp"
#include "BoardConfig.hpp"

template <class TMemory = Board::FRAM_MemoryType>
class DBStorage : public Singleton<DBStorage<>>
{
	public:
		DBStorage(Board::MemoryParams_t &MemoryParams = Board::FRAM_MemoryParams);
		~DBStorage();
		void SetBuff(uint8_t *buff, size_t len);
		void GetBuff(uint8_t *buff, size_t len);

	private:
		TMemory *BoardMemory;
};

template <class TMemory>
DBStorage<TMemory>::DBStorage(Board::MemoryParams_t &MemoryParams)
{
	BoardMemory = new TMemory(MemoryParams);
}

template <class TMemory>
DBStorage<TMemory>::~DBStorage()
{
	delete BoardMemory;
}

template <class TMemory>
void DBStorage<TMemory>::SetBuff(uint8_t *buff, size_t len)
{
	BoardMemory->Write(0x20, buff, len);
}

template <class TMemory>
void DBStorage<TMemory>::GetBuff(uint8_t *buff, size_t len)
{
	BoardMemory->Read(0x20, buff, len);
}
