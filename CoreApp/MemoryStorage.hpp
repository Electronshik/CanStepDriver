#pragma once
#include "main.hpp"
#include "BoardMemory.hpp"
#include "BoardConfig.hpp"
#include "StorableSettings.hpp"
#include <string.h>

template <class TMemory>
class Memory
{
	public:
		Memory(Board::MemoryParams_t &MemoryParams);
		~Memory();
		void SetBuff(uint8_t *buff, size_t len);
		void GetBuff(uint8_t *buff, size_t len);
		void RestoreSetting(IStorableSetting *Setting);
		void SaveSetting(IStorableSetting *Setting);

	private:
		TMemory *BoardMemory;
		bool CheckTagsAndCrc(IStorableSetting *Setting);
};

template <class TMemory>
Memory<TMemory>::Memory(Board::MemoryParams_t &MemoryParams)
{
	BoardMemory = new TMemory(MemoryParams);
}

template <class TMemory>
Memory<TMemory>::~Memory()
{
	delete BoardMemory;
}

template <class TMemory>
void Memory<TMemory>::SetBuff(uint8_t *buff, size_t len)
{
	BoardMemory->Write(0x1000, buff, len);
}

template <class TMemory>
void Memory<TMemory>::GetBuff(uint8_t *buff, size_t len)
{
	BoardMemory->Read(0x1000, buff, len);
}

template <class TMemory>
void Memory<TMemory>::RestoreSetting(IStorableSetting *Setting)
{
	auto Readed = (IStorableSetting*) new uint8_t[Setting->Size];
	BoardMemory->Read(Setting->StoreAddr, (uint8_t*)Readed, Setting->Size);

	if(this->CheckTagsAndCrc(Readed))
	{
		memcpy((void*)Setting, (void*)Readed, Setting->Size); //include <string.h> for -fpermissive warning fix
	}

	delete[] Readed;
}

template <class TMemory>
void Memory<TMemory>::SaveSetting(IStorableSetting *Setting)
{
	auto Readed = (IStorableSetting*) new uint8_t[Setting->Size];
	BoardMemory->Read(Setting->StoreAddr, (uint8_t*)Readed, sizeof(IStorableSetting));

	auto newWritingTag = 0;
	if(this->CheckTagsAndCrc(Readed))
	{
		newWritingTag = Readed->CheckTagBegin;
	}
	newWritingTag++;
	Setting->CheckTagBegin = newWritingTag;
	Setting->CheckTagEnd = newWritingTag;
	BoardMemory->Write(Setting->StoreAddr, (uint8_t*)Setting, Setting->Size);
	delete[] Readed;
}

template <class TMemory>
bool Memory<TMemory>::CheckTagsAndCrc(IStorableSetting *Setting)
{
	if(	(Setting->CheckTagBegin == Setting->CheckTagEnd) &&
		(Setting->CheckTagBegin != BoardMemory->MemoryParams.ErasedVal) &&
		(Setting->CheckTagBegin != 0x00) && (Setting->CheckTagBegin != 0xff))
	{
		return true;
	}
	else
	{
		return false;
	}
}		
