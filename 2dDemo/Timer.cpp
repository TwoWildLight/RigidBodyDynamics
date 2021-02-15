#include "Timer.h"

Timer::Timer()
{
	previous = HRClock::now();
}

float Timer::Peek() const
{
	return std::chrono::duration<float>(HRClock::now() - previous).count();
}

float Timer::Mark()
{
	float fDeltaTime = Peek();
	previous = HRClock::now();
	return fDeltaTime;
}