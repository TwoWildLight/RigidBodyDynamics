#pragma once
#include "Body.h"
#include <memory>

class Physics
{
public:
	struct CollisionData
	{
		bool bCollided;
		math::Vector vNormal;
		float fDepth;
	};
public:
	void ApplyPhysics(Body& body);
	CollisionData CheckCollision(Body& lhs, Body& rhs);
	void HandleCollision(Body& lhs, Body& rhs);
private:
	static constexpr float fLinearProjection = 0.8f;
	static constexpr float fSlop = 0.01f;
	float fGravity = 600.0f;
	float fLinearDampling = 0.0015f;
	float fAngularDampling = 0.002f;
};