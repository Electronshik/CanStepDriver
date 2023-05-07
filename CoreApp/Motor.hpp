#include "main.hpp"

constexpr uint32_t MotorPosMin = 0;
constexpr uint32_t MotorPosMax = 1000;

enum class MotorDir: uint8_t
{
	UP, DOWN
};

class MotorVirtual
{
	public:
		MotorVirtual() {};
		void Move(MotorDir Dir, uint32_t steps)
		{
			if(Dir == MotorDir::UP)
			{
				Position = (Position + steps) < MotorPosMax ? (Position + steps) : MotorPosMax;
			}
			else
			{
				Position = (Position >= steps) && ((Position - steps) > MotorPosMin) ? (Position - steps) : MotorPosMin;
			}
		};
		void Stop() {};
	private:
		uint32_t Position;
};
