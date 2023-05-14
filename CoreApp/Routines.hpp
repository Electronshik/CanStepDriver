#pragma once
#include "main.hpp"
#include "OS_Wrappers.hpp"

namespace Routines
{
	namespace Led
	{
		inline constexpr OS::TName Name = "Led Indication";
		inline constexpr OS::TStack Stack = 128;
		inline constexpr OS::TPrio Prio = 0;

		void Routine(void *pvParameters);
	}

	namespace Main
	{
		inline constexpr OS::TName Name = "Main Thread";
		inline constexpr OS::TStack Stack = 256;
		inline constexpr OS::TPrio Prio = 0;

		void Routine(void *pvParameters);
	}

	namespace Interface
	{
		inline constexpr OS::TName Name = "Interface Handle";
		inline constexpr OS::TStack Stack = 512;
		inline constexpr OS::TPrio Prio = 0;

		void Routine(void *pvParameters);
	}
}
