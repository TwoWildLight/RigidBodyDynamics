#pragma once
#include "TwlMath.h"
#include "Graphics.h"

class Body
{
	friend class Physics;
public:
	enum class Type
	{
		btStatic,
		btDynamic
	};
public:
	Body(Type type,
		math::Mesh mesh_in,
		const math::Vector& vPosition,
		const math::Vector& vAngle,
		float fMass,
		float fFriction,
		float fRestitution,
		float fInertia);
public:
	Type GetType() const;
	const math::Mesh& GetMesh() const;
	math::Vector GetPosition() const;
	math::Vector GetAngle() const;
	void Integrate(float fDeltaTime);
private:
	Type type;
	math::Mesh mesh;
	math::Vector vPosition;
	math::Vector vAngle;
	math::Vector vVelocity;
	math::Vector vAngularVelocity;
	math::Vector vForce;
	math::Vector vTorque;
	float fMass;
	float fInverseMass;
	float fFriction;
	float fRestitution;
	float fInertia;
	float fInverseInertia;
};