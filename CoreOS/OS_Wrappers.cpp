#include "OS_Wrappers.hpp"
#include <new>
#include "FreeRTOS.h"
#include "portable.h"
#include "task.h"

uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((section (".ccmram")));

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

	void Suspend()
	{
		vTaskSuspendAll();
	}

	void Resume()
	{
		xTaskResumeAll();
	}

	Thread::Thread(void(*func)(void*), const char *name, uint16_t stack_size, void *param, uint32_t prio)
	{
		BaseType_t xReturned = xTaskCreate((TaskFunction_t)func, name,
			stack_size, (void*)param, prio, (TaskHandle_t *const)(&(this->Handle)));
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
}
