#pragma once
#include "Physics.h"
#include <vector>
#include <memory>

class Body;

class World
{
public:
	void AddBody(std::unique_ptr<Body> pBody);
	void Update(float fDeltaTime);
	void Render(Graphics& gfx);
	void Clear();
private:
	Physics physics;
	std::vector<std::unique_ptr<Body>> bodyPtrs;
};