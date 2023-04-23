#pragma once
#include <cstddef>
#include <cstdint>
#include "StorableSettings.hpp"

enum class ErrorCode
{
	OK,
	ERR
};

extern GeneralSetting GenSettings;
extern MotorSetting MotorSettings;
