#pragma once
#include "main.hpp"

struct IStorableSetting
{
	uint32_t CheckTagBegin;
	const uint32_t StoreAddr;
	const uint32_t Size;
	uint32_t CheckTagEnd;

	protected:
		IStorableSetting(uint32_t Addr, uint32_t Size) : StoreAddr(Addr), Size(Size) {}
};

struct GeneralSetting : public IStorableSetting
{
	uint32_t Mark;
	char str[15];
	GeneralSetting(uint32_t Addr) : IStorableSetting(Addr, sizeof(*this)) {}
};

struct MotorSetting : public IStorableSetting
{
	// using IStorableSetting::IStorableSetting;
	uint32_t Mark;
	char str[20];
	MotorSetting(uint32_t Addr) : IStorableSetting(Addr, sizeof(*this)) {}
};
