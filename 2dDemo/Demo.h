#pragma once
#include "Window.h"
#include "Timer.h"

namespace twl
{
	class Demo : public Window
	{
	public:
		Demo();
	public:
		void Initiate();
	private:
		void Update(float fDeltaTime);
		void Render();
	private:
		Timer timer;
	};
}