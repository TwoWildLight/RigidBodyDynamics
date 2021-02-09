#pragma once
#include <chrono>

namespace twl
{
	class Timer
	{
		using HRClock = std::chrono::high_resolution_clock;
	public:
		Timer();
		virtual ~Timer() = default;
	public:
		float Peek() const;
		float Mark();
	private:
		HRClock::time_point previous;
	};
}