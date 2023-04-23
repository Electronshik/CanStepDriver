#include "OS_Wrappers.hpp"
#include <new>
#include "FreeRTOS.h"
#include "portable.h"
#include "task.h"

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

	Thread::Thread(void(*func)(void*), const char *name, uint16_t stack_size, void *param, uint32_t prio)
	{
		BaseType_t xReturned = xTaskCreate(
			(TaskFunction_t)func,	/* Function that implements the task. */
			name,				/* Text name for the task. */
			stack_size,			/* Stack size in words, not bytes. */
			(void*)param,		/* Parameter passed into the task. */
			prio,				/* Priority at which the task is created. */
			(TaskHandle_t *const)(&(this->Handle)));	/* Used to pass out the created task's handle. */
	}

	Thread::~Thread()
	{
		vTaskDelete((TaskHandle_t)this->Handle);
	}

}
