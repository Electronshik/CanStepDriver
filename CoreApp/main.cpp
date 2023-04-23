#include "main.hpp"
#include "Routines.hpp"
#include "Board.hpp"
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
	Board::RS485::Transmit((uint8_t*)ptr, len);
	return 1;
}
/*
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
   static StaticTask_t xIdleTaskTCB;
   static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];
 
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
   *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
   static StaticTask_t xTimerTaskTCB;
   static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];
 
   *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
   *ppxTimerTaskStackBuffer = uxTimerTaskStack;
   *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
*/
#ifdef __cplusplus
}
#endif

// echo /nul | arm-none-eabi-g++.exe -x c++ -dM -E - > _builtin_defines.hpp
// printf("is lock free? %s\n", std::atomic<int>::is_always_lock_free ? "true" : "false");

/*
#ifdef __cplusplus
extern "C" {
#endif

extern int _end;

void *_sbrk(int incr) {
  static unsigned char *heap = NULL;
  unsigned char *prev_heap;

  if (heap == NULL) {
    heap = (unsigned char *)&_end; //&_end; //0x20030000
  }
  prev_heap = heap;

  heap += incr;

  return prev_heap;
}

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
//   st->st_mode = S_IFCHR;

  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

void _exit(int status) {
  __asm("BKPT #0");
}

void _kill(int pid, int sig) {
  return;
}

int _getpid(void) {
  return -1;
}

int _read (int file, char * ptr, int len) {
	return 0;
}

#ifdef __cplusplus
}
#endif
*/