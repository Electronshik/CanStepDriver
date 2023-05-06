#include "main.hpp"
#include "Motor.hpp"
#include <concepts>

template <class T> concept StepMotor =
requires(T t)
{
	{ T() };
	{ ::new(T) };
	{ t.Move(uint32_t{}) } -> std::same_as<void>;
	{ t.Stop() } -> std::same_as<void>;
};

template <StepMotor T>
class MotorController
{
	public:
		MotorController();
		~MotorController();
		void GoTo(uint32_t value);
	private:
		T* Motor;
};

template <StepMotor T>
MotorController<T>::MotorController()
{
	this->Motor = new(T);
}

template <StepMotor T>
MotorController<T>::~MotorController()
{
}

template <StepMotor T>
void MotorController<T>::GoTo(uint32_t value)
{
	Motor.Move(value);
	Motor.Stop();
}
