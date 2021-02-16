#include "Physics.h"
#include <algorithm>

void Physics::ApplyPhysics(Body& body)
{
	if (body.GetType() != Body::Type::btStatic)
	{
		body.vForce += math::Vector(0.0f, -fGravity);
		body.vForce -= body.vVelocity * fLinearDampling;
	}
}

Physics::CollisionData Physics::CheckCollision(Body& lhs, Body& rhs)
{
	// rotate both bodies
	auto lhsVertices = math::TransformVertices(lhs.mesh.vertices, math::Matrix::Rotation(lhs.GetAngle()));
	auto rhsVertices = math::TransformVertices(rhs.mesh.vertices, math::Matrix::Rotation(rhs.GetAngle()));

	// normals of both meshes
	const auto lhsNormals = math::Extract2DNormals({ lhsVertices, lhs.mesh.indices });
	const auto rhsNormals = math::Extract2DNormals({ rhsVertices, rhs.mesh.indices });

	// translate both bodies
	lhsVertices = math::TransformVertices(lhsVertices, math::Matrix::Translation(lhs.GetPosition()));
	rhsVertices = math::TransformVertices(rhsVertices, math::Matrix::Translation(rhs.GetPosition()));

	std::vector<math::Vector> normals;
	normals.insert(normals.end(), lhsNormals.begin(), lhsNormals.end());
	normals.insert(normals.end(), rhsNormals.begin(), rhsNormals.end());
	const auto penetration = math::GetMinimumPenetration(normals, lhsVertices, rhsVertices);

	if (penetration)
	{
		const auto& vNormal = penetration->first.Dot(rhs.GetPosition() - lhs.GetPosition()) < 0.0f ? -penetration->first : penetration->first;
		return { true, vNormal, penetration->second };
	}

	return { false };
}

void Physics::HandleCollision(Body& lhs, Body& rhs)
{
	const auto collisionData = CheckCollision(lhs, rhs);
	if (collisionData.bCollided)
	{
		float fRestitution = std::min(lhs.fRestitution, rhs.fRestitution);
		math::Vector vRelativeVelocity = rhs.vVelocity - lhs.vVelocity;
		float fVelocityDot = vRelativeVelocity.Dot(collisionData.vNormal);

		if (fVelocityDot > 0.0f)
		{
			return;
		}

		float fImpulseMagnitude = (-(1 + fRestitution) * fVelocityDot) / (lhs.fInverseMass + rhs.fInverseMass);
		math::Vector vImpulse = collisionData.vNormal * fImpulseMagnitude;
		if (lhs.GetType() == Body::Type::btDynamic) lhs.vVelocity -= vImpulse * lhs.fInverseMass;
		if (rhs.GetType() == Body::Type::btDynamic) rhs.vVelocity += vImpulse * rhs.fInverseMass;
		//lhs.vAngularVelocity += math::Vector(DirectX::XMVector2Cross(collisionData.vPoint, vImpulse)) * lhs.fInverseInertia;
		//rhs.vAngularVelocity += math::Vector(DirectX::XMVector2Cross(collisionData.vPoint, vImpulse)) * rhs.fInverseInertia;

		math::Vector vCorrection = collisionData.vNormal * std::max(collisionData.fDepth, fSlop) / (lhs.fInverseMass + rhs.fInverseMass) * fLinearProjection;
		if (lhs.GetType() == Body::Type::btDynamic) lhs.vPosition -= vCorrection * lhs.fInverseMass;
		if (rhs.GetType() == Body::Type::btDynamic) rhs.vPosition += vCorrection * rhs.fInverseMass;

		vRelativeVelocity = rhs.vVelocity - lhs.vVelocity;
		math::Vector vTangent = math::Vector::Normalize2d(vRelativeVelocity - collisionData.vNormal * (vRelativeVelocity.Dot(collisionData.vNormal)));
		float fFrictionMagnitude = -vTangent.Dot(vRelativeVelocity) / (lhs.fInverseMass + rhs.fInverseMass);
		float fFrictionSq = math::Squared(lhs.fFriction) + math::Squared(rhs.fFriction);
		math::Vector vFriction = abs(fFrictionMagnitude) < fImpulseMagnitude * fFrictionSq ? vTangent * fFrictionMagnitude : vTangent * -fImpulseMagnitude * fFrictionSq;
		if (lhs.GetType() == Body::Type::btDynamic) lhs.vVelocity -= vFriction * lhs.fInverseMass;
		if (rhs.GetType() == Body::Type::btDynamic) rhs.vVelocity += vFriction * rhs.fInverseMass;
	}
}