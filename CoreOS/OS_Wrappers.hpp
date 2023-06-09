#pragma once
#include "main.hpp"

namespace OS
{

	void Start();
	void Delay(const uint32_t delay);
	void Suspend();
	void Resume();

	using TName = const char *;
	using TStack = uint16_t;
	using TPrio = uint32_t;

	class Thread
	{
		public:
			Thread(void(*func)(void*), const char *name, uint16_t stack_size, void *param, uint32_t prio);
			~Thread();
			void Suspend();
			void Resume();

		private:
			void *Handle;
	};

}
