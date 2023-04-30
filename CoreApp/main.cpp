#include "main.hpp"
#include "Routines.hpp"
#include "Board.hpp"
#include "BoardConfig.hpp"
#include "BoardInterface.hpp"
#include "Globals.hpp"
#include "OS_Wrappers.hpp"
#include <atomic>
#include <cstdio>

using namespace Routines;

Globals Global;

int main(void)
{
	Board::Init();

	setvbuf(stdout, NULL, _IONBF, 0);

	Global.State = GlState::MOVING;

	auto LedTask = new OS::Thread(Led::Routine, Led::Name, Led::Stack, 0, Led::Prio);
	auto MainTask = new OS::Thread(Main::Routine, Main::Name, Main::Stack, 0, Main::Prio);
	auto InterfaceTask = new OS::Thread(Interface::Routine, Interface::Name, Interface::Stack, 0, Interface::Prio);

	OS::Start();
}

#ifdef __cplusplus
extern "C" {
#endif

int _write(int fd, char* ptr, int len)
{
	Board::InterfaceType::GetInstance()->Transmit((uint8_t*)ptr, len);
	return 1;
}

#ifdef __cplusplus
}
#endif

// echo /nul | arm-none-eabi-g++.exe -x c++ -dM -E - > _builtin_defines.hpp
// printf("is lock free? %s\n", std::atomic<int>::is_always_lock_free ? "true" : "false");
