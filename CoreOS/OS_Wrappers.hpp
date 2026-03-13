#pragma once
#include "main.hpp"

namespace OS
{
	using Bool = bool;
	inline constexpr Bool True = true;
	inline constexpr Bool False = false;

	void Start();
	void Delay(const uint32_t delay);
	void DelayUntil(uint32_t* lastWakeTime, uint32_t freq);
	void Suspend();
	void Resume();
	auto GetTickCount() -> uint32_t;

	void EnterCritial();
	void ExitCritial();
	auto EnterCritialFromISR() -> uint32_t;
	void ExitCritialFromISR(uint32_t prio);

	using TName = const char*;
	using TStack = uint16_t;
	using TPrio = uint32_t;

	class Thread
	{
		public:
			Thread() = delete;

			Thread(void (*func)(void*), const char* name, uint16_t stack_size, void* param, uint32_t prio);
			~Thread();
			void Suspend();
			void Resume();
			auto GetStackWatarMark() -> uint32_t;

		private:
			void* Handle;
	};

	using TimerClbk = void (*)(void*);

	class Timer
	{
		public:
			Timer() = delete;

			Timer(const char* name, uint32_t period, OS::Bool autoreload, TimerClbk tmr_clbk);
			~Timer();
			auto Start() -> OS::Bool;
			auto Stop() -> OS::Bool;
			auto Reset() -> OS::Bool;
			auto IsActive() -> OS::Bool;
			auto ChangePeriod(uint32_t newPeriod) -> OS::Bool;

		private:
			void* Handle;
	};

	class Mutex
	{
		public:
			Mutex();
			~Mutex();
			auto Take(uint32_t ticks) -> OS::Bool;
			auto TakeFromISR() -> OS::Bool;
			void Give();
			void GiveFromISR();

		private:
			void* Handle;
	};

	class StreamBuffer
	{
		public:
			StreamBuffer(uint32_t size);
			StreamBuffer(uint32_t size, uint32_t triggerLvl);
			~StreamBuffer();
			auto Send(uint8_t* buff, uint32_t len) -> OS::Bool;
			auto SendFromISR(uint8_t* buff, uint32_t len) -> OS::Bool;
			auto Receive(uint8_t* buff, uint32_t len, uint32_t ticks) -> uint32_t;
			auto ReceiveFromISR(uint8_t* buff, uint32_t len) -> uint32_t;

		private:
			void* Handle;
	};

	class GenericQueue
	{
		public:
			GenericQueue() = delete;

			GenericQueue(uint16_t size, uint16_t itemSize);
			~GenericQueue();
			auto Send(void* buff) -> OS::Bool;
			auto SendFromISR(void* buff) -> OS::Bool;
			auto Receive(void* buff, uint32_t ticksToWait) -> OS::Bool;
			auto ReceiveFromISR(void* buff) -> OS::Bool;

		protected:
			void* Handle;
	};

	template <typename T>
	class Queue : public GenericQueue
	{
		public:
			Queue(uint16_t size) : GenericQueue(size, sizeof(T))
			{
			}

			~Queue()
			{
			}

			auto Send(T* ptr) -> OS::Bool
			{
				return GenericQueue::Send(static_cast<void*>(ptr));
			}

			auto Send(T val) -> OS::Bool
			{
				return this->Send(&val);
			}

			auto SendFromISR(T* ptr) -> OS::Bool
			{
				return GenericQueue::SendFromISR(static_cast<void*>(ptr));
			}

			auto SendFromISR(T val) -> OS::Bool
			{
				return this->SendFromISR(&val);
			}

			auto Receive(T* val, uint32_t ticks) -> OS::Bool
			{
				return GenericQueue::Receive(static_cast<void*>(val), ticks);
			}

			auto ReceiveFromISR(T* val) -> OS::Bool
			{
				return GenericQueue::ReceiveFromISR(static_cast<void*>(val));
			}
	};

} // namespace OS
