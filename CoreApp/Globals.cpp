#include "Globals.hpp"

etl::queue<GlCmd, 32> GlCmdQueue;

Globals::Globals()
{
	this->State = GlState::IDLE;
	this->OldState = GlState::ERROR;;
}

Globals::~Globals()
{
}
