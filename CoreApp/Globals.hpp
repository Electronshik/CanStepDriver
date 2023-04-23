#pragma once
#include "main.hpp"
#include "etl/queue.h"

enum class GlState
{
	IDLE,
	MOVING,
	ERROR
};

class Globals
{
	private:
		GlState OldState;

	public:
		GlState State;
		Globals();
		~Globals();

	GlState& operator=(GlState& state)
	{
		this->OldState = this->State;
		// this->State = state;
		return state;
	}
};

extern Globals Global;

enum class GlCmdType
{
	NO,
	LED
};

struct GlCmd
{
	GlCmdType Type;
	uint32_t Value;
};

extern etl::queue<GlCmd, 32> GlCmdQueue;
