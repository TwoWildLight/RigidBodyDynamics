#pragma once
#include "Window.h"
#include "Timer.h"
#include "World.h"

class Demo : public Window
{
public:
	Demo();
public:
	void Initiate();
private:
	void Update(float fDeltaTime);
	void Render();
	void SpawnBody();
private:
	Timer timer;
	World world;
};
