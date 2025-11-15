#include "OS_Wrappers.hpp"
#include "FreeRTOS.h"
#include "portable.h"
#include "portmacro.h"
#include "queue.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "task.h"
#include "timers.h"
#include <new>

uint8_t ucHeap[configTOTAL_HEAP_SIZE]; // __attribute__((section(".ccmram")));

void* operator new(size_t size)
{
	return pvPortMalloc(size);
	// throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void* operator new[](size_t size)
{
	return pvPortMalloc(size);
	// throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void operator delete(void* ptr)
{
	vPortFree(ptr);
}

void operator delete(void* ptr, size_t size)
{
	vPortFree(ptr);
}

void operator delete[](void* ptr)
{
	vPortFree(ptr);
}

void operator delete[](void* ptr, size_t size)
{
	vPortFree(ptr);
}

namespace OS
{
	void Start()
	{
		vTaskStartScheduler();
	}

	void Delay(const uint32_t delay)
	{
		vTaskDelay(delay);
	}

	void DelayUntil(uint32_t* lastWakeTime, uint32_t freq)
	{
		vTaskDelayUntil((TickType_t*)lastWakeTime, (TickType_t)freq);
	}

	void Suspend()
	{
		vTaskSuspendAll();
	}

	void Resume()
	{
		xTaskResumeAll();
	}

	auto GetTickCount() -> uint32_t
	{
		return (uint32_t)xTaskGetTickCount();
	}

	void EnterCritial()
	{
		taskENTER_CRITICAL();
	}

	void ExitCritial()
	{
		taskEXIT_CRITICAL();
	}

	auto EnterCritialFromISR() -> uint32_t
	{
		return (uint32_t)taskENTER_CRITICAL_FROM_ISR();
	}

	void ExitCritialFromISR(uint32_t prio)
	{
		taskEXIT_CRITICAL_FROM_ISR((UBaseType_t)prio);
	}

	Thread::Thread(void (*func)(void*), const char* name, uint16_t stack_size, void* param, uint32_t prio)
	{
		BaseType_t xReturned = xTaskCreate((TaskFunction_t)func, name, stack_size, (void*)param, prio,
										   (TaskHandle_t* const)(&(this->Handle)));
	}

	Thread::~Thread()
	{
		vTaskDelete((TaskHandle_t)this->Handle);
	}

	void Thread::Suspend()
	{
		vTaskSuspend((TaskHandle_t)this->Handle);
	}

	void Thread::Resume()
	{
		vTaskResume((TaskHandle_t)this->Handle);
	}

	auto Thread::GetStackWatarMark() -> uint32_t
	{
		return (uint32_t)uxTaskGetStackHighWaterMark((TaskHandle_t)this->Handle);
	}

	Timer::Timer(const char* name, uint32_t period, OS::Bool autoreload, TimerClbk tmr_clbk)
	{
		TimerHandle_t xTimer =
			xTimerCreate(name, period, autoreload ? pdTRUE : pdFALSE, (void*)0, (void (*)(TimerHandle_t))tmr_clbk);
		this->Handle = (void*)xTimer;
	}

	Timer::~Timer()
	{
		xTimerDelete((TimerHandle_t)this->Handle, 0);
	}

	auto Timer::Start() -> OS::Bool
	{
		return (xTimerStart((TimerHandle_t)this->Handle, 0) == pdPASS) ? OS::True : OS::False;
	}

	auto Timer::Stop() -> OS::Bool
	{
		return (xTimerStop((TimerHandle_t)this->Handle, 0) == pdPASS) ? OS::True : OS::False;
	}

	auto Timer::Reset() -> OS::Bool
	{
		return (xTimerReset((TimerHandle_t)this->Handle, 0) == pdPASS) ? OS::True : OS::False;
	}

	auto Timer::IsActive() -> OS::Bool
	{
		return (xTimerIsTimerActive((TimerHandle_t)this->Handle) == pdTRUE) ? OS::True : OS::False;
	}

	auto Timer::ChangePeriod(uint32_t newPeriod) -> OS::Bool
	{
		return (xTimerChangePeriod((TimerHandle_t)this->Handle, newPeriod, 0) == pdPASS) ? OS::True : OS::False;
	}

	Mutex::Mutex()
	{
		SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();
		this->Handle = (void*)xSemaphore;
	}

	Mutex::~Mutex()
	{
		vSemaphoreDelete((SemaphoreHandle_t)this->Handle);
	}

	auto Mutex::Take(uint32_t ticks) -> OS::Bool
	{
		return (xSemaphoreTake((SemaphoreHandle_t)this->Handle, (TickType_t)ticks) == pdTRUE) ? OS::True : OS::False;
	}

	auto Mutex::TakeFromISR() -> OS::Bool
	{
		portBASE_TYPE xTaskWoken;
		if (xSemaphoreTakeFromISR((SemaphoreHandle_t)this->Handle, &xTaskWoken) == pdTRUE)
		{
			if (xTaskWoken == pdTRUE)
				taskYIELD();
			return OS::True;
		}
		else
		{
			return OS::False;
		}
	}

	void Mutex::Give()
	{
		xSemaphoreGive((SemaphoreHandle_t)this->Handle);
	}

	void Mutex::GiveFromISR()
	{
		portBASE_TYPE xTaskWoken;
		xSemaphoreGiveFromISR((SemaphoreHandle_t)this->Handle, &xTaskWoken);
		if (xTaskWoken == pdTRUE)
			taskYIELD();
	}

	StreamBuffer::StreamBuffer(uint32_t size) : StreamBuffer(size, 1)
	{
	}

	StreamBuffer::StreamBuffer(uint32_t size, uint32_t triggerLvl)
	{
		StreamBufferHandle_t xStreamBuffer = xStreamBufferCreate(size, triggerLvl);
		this->Handle = (void*)xStreamBuffer;
	}

	StreamBuffer::~StreamBuffer()
	{
		vStreamBufferDelete((StreamBufferHandle_t)this->Handle);
	}

	auto StreamBuffer::Send(uint8_t* buff, uint32_t len) -> OS::Bool
	{
		uint32_t bytesSent = xStreamBufferSend((StreamBufferHandle_t)this->Handle, (void*)buff, len, 0);
		return (bytesSent == len) ? OS::True : OS::False;
	}

	auto StreamBuffer::SendFromISR(uint8_t* buff, uint32_t len) -> OS::Bool
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		uint32_t bytesSent =
			xStreamBufferSendFromISR((StreamBufferHandle_t)this->Handle, (void*)buff, len, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken == pdTRUE)
			taskYIELD();

		return (bytesSent == len) ? OS::True : OS::False;
	}

	auto StreamBuffer::Receive(uint8_t* buff, uint32_t len, uint32_t ticks) -> uint32_t
	{
		return xStreamBufferReceive((StreamBufferHandle_t)this->Handle, (void*)buff, len, ticks);
	}

	auto StreamBuffer::ReceiveFromISR(uint8_t* buff, uint32_t len) -> uint32_t
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		uint32_t receivedBytes = xStreamBufferReceiveFromISR((StreamBufferHandle_t)this->Handle, (void*)buff, len,
															 &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken == pdTRUE)
			taskYIELD();

		return receivedBytes;
	}

	GenericQueue::GenericQueue(uint16_t size, uint16_t itemSize)
	{
		this->Handle = (void*)xQueueCreate(size, itemSize);
	}

	GenericQueue::~GenericQueue()
	{
		vQueueDelete((QueueHandle_t)this->Handle);
	}

	auto GenericQueue::Send(void* buff) -> OS::Bool
	{
		return (xQueueSend((QueueHandle_t)this->Handle, buff, 0) == pdTRUE) ? OS::True : OS::False;
	}

	auto GenericQueue::SendFromISR(void* buff) -> OS::Bool
	{
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		BaseType_t res = xQueueSendFromISR((QueueHandle_t)this->Handle, buff, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken)
			taskYIELD();

		return (res == pdPASS) ? OS::True : OS::False;
	}

	auto GenericQueue::Receive(void* buff, uint32_t ticksToWait) -> OS::Bool
	{
		return (xQueueReceive((QueueHandle_t)this->Handle, (void*)buff, ticksToWait) == pdTRUE) ? OS::True : OS::False;
	}

	auto GenericQueue::ReceiveFromISR(void* buff) -> OS::Bool
	{
		BaseType_t xTaskWokenByReceive = pdFALSE;

		BaseType_t res = xQueueReceiveFromISR((QueueHandle_t)this->Handle, (void*)buff, &xTaskWokenByReceive);
		if (xTaskWokenByReceive != pdFALSE)
			taskYIELD();

		return (res == pdPASS) ? OS::True : OS::False;
	}

} // namespace OS

#if configSUPPORT_STATIC_ALLOCATION
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer,
								   uint32_t* pulIdleTaskStackSize)
{
	/* If the buffers to be provided to the Idle task are declared inside this
	function then they must be declared static - otherwise they will be allocated on
	the stack which is no longer guaranteed to exist once this function returns. */
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

	/* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	TCB will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out a pointer to the array that will be used as the Idle task's
	stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array that is used as the Idle task's stack. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer,
									uint32_t* pulTimerTaskStackSize)
{
	/* If the buffers to be provided to the Timer task are declared inside this
	function then it is possible that the compiler will allocate them on the
	stack, rather than in an area that can be guaranteed to persist for the
	lifetime of the Timer task.  Therefore the buffers must be declared static. */
	static StaticTask_t xTimerTaskTCB;
	static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's TCB will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out a pointer to the array that will be used as the Timer task's
	stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array that is used as the Timer task's stack. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif /* configSUPPORT_STATIC_ALLOCATION */
