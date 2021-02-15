#include "Body.h"

Body::Body(Type type, math::Mesh mesh_in, const math::Vector& vPosition, const math::Vector& vAngle, float fMass, float fFriction, float fRestitution, float fInertia)
	:
	type(type),
	mesh(std::move(mesh_in)),
	vPosition(vPosition),
	vAngle(vAngle),
	fMass(fMass),
	fFriction(fFriction),
	fRestitution(fRestitution),
	fInertia(fInertia)
{
	if (fMass == 0.0f) fInverseMass = 0.0f;
	else fInverseMass = 1.0f / fMass;

	if (fInertia == 0.0f) fInverseInertia = 0.0f;
	else fInverseInertia = 1.0f / fInertia;
}

Body::Type Body::GetType() const
{
	return type;
}

const math::Mesh& Body::GetMesh() const
{
	return mesh;
}

math::Vector Body::GetPosition() const
{
	return vPosition;
}

math::Vector Body::GetAngle() const
{
	return vAngle;
}

void Body::Integrate(float fDeltaTime)
{
	vVelocity += (vForce * fInverseMass) * fDeltaTime;
	memset(&vForce, 0, sizeof(vForce));
	vPosition += vVelocity * fDeltaTime;

	vAngularVelocity += (vTorque * fInverseInertia) * fDeltaTime;
	memset(&vTorque, 0, sizeof(vTorque));
	vAngle += vAngularVelocity * fDeltaTime;
}