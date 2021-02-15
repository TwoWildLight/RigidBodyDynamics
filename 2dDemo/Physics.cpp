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
	std::vector<math::Vector> lhsVertices = lhs.mesh.vertices;
	std::vector<math::Vector> rhsVertices = rhs.mesh.vertices;
	{
		const auto mLhsRotation = math::Matrix::Rotation(lhs.GetAngle());
		for (auto& v : lhsVertices)
		{
			v *= mLhsRotation;
		}

		const auto mRhsRotation = math::Matrix::Rotation(rhs.GetAngle());
		for (auto& v : rhsVertices)
		{
			v *= mRhsRotation;
		}
	}

	// normals of both meshes
	std::vector<math::Vector> normals;
	{
		for (size_t i = 0; i < lhs.mesh.indices.size(); i += 2)
		{
			math::Vector vLine = lhsVertices[lhs.mesh.indices[i + 1]] - lhsVertices[lhs.mesh.indices[i]];
			math::Vector n(vLine.y, -vLine.x);
			n.Normalize2d();
			normals.push_back(n.Dot(lhsVertices[lhs.mesh.indices[i]] + vLine * 0.5f) > 0.0f ? n : -n);
		}

		for (size_t i = 0; i < rhs.mesh.indices.size(); i += 2)
		{
			math::Vector vLine = rhsVertices[rhs.mesh.indices[i + 1]] - rhsVertices[rhs.mesh.indices[i]];
			math::Vector n(vLine.y, -vLine.x);
			n.Normalize2d();
			normals.push_back(n.Dot(rhsVertices[rhs.mesh.indices[i]] + vLine * 0.5f) > 0.0f ? n : -n);
		}
	}

	// translate both bodies
	{
		const auto mLhsTranslation = math::Matrix::Translation(lhs.GetPosition());
		for (auto& v : lhsVertices)
		{
			v *= mLhsTranslation;
		}

		const auto mRhsTranslation = math::Matrix::Translation(rhs.GetPosition());
		for (auto& v : rhsVertices)
		{
			v *= mRhsTranslation;
		}
	}

	constexpr float fMax = std::numeric_limits<float>::max();
	constexpr float fMin = std::numeric_limits<float>::lowest();
	math::Vector vNormal;
	float fPenetrationDepth = fMax;
	for (const auto& n : normals)
	{
		std::pair<float, float> lhsProjection = { fMax, fMin };
		std::pair<float, float> rhsProjection = { fMax, fMin };

		for (const auto& v : lhsVertices)
		{
			float fDot = n.Dot(v);
			if (fDot < lhsProjection.first) lhsProjection.first = fDot;
			if (fDot > lhsProjection.second) lhsProjection.second = fDot;
		}

		for (const auto& v : rhsVertices)
		{
			float fDot = n.Dot(v);
			if (fDot < rhsProjection.first) rhsProjection.first = fDot;
			if (fDot > rhsProjection.second) rhsProjection.second = fDot;
		}

		std::pair<float, float> depth = { lhsProjection.second - rhsProjection.first, rhsProjection.second - lhsProjection.first };
		if (depth.first < 0.0f || depth.second < 0.0f) return { false };

		float fDepth = std::min(depth.first, depth.second);
		if (fPenetrationDepth > fDepth)
		{
			vNormal = n;
			fPenetrationDepth = abs(fDepth);
		}
	}

	return { true, vNormal, fPenetrationDepth };
}

void Physics::HandleCollision(Body& lhs, Body& rhs)
{
	const auto collisionData = CheckCollision(lhs, rhs);
	if (collisionData.bCollided)
	{
		float fRestitution = std::min(lhs.fRestitution, rhs.fRestitution);
		math::Vector vRelativeVelocity = rhs.vVelocity - lhs.vVelocity;
		float fVelocityDot = vRelativeVelocity.Dot(collisionData.vNormal);

		if (fVelocityDot > 0.01f)
			return;

		float fImpulseMagnitude = (-(1 + fRestitution) * fVelocityDot) / (lhs.fInverseMass + rhs.fInverseMass);
		math::Vector vImpulse = collisionData.vNormal * fImpulseMagnitude;
		lhs.vVelocity -= vImpulse * lhs.fInverseMass;
		rhs.vVelocity += vImpulse * rhs.fInverseMass;
		//lhs.vAngularVelocity += math::Vector(DirectX::XMVector2Cross(collisionData.vPoint, vImpulse)) * lhs.fInverseInertia;
		//rhs.vAngularVelocity += math::Vector(DirectX::XMVector2Cross(collisionData.vPoint, vImpulse)) * rhs.fInverseInertia;

		math::Vector vCorrection = collisionData.vNormal * std::max(collisionData.fDepth, fSlop) / (lhs.fInverseMass + rhs.fInverseMass) * fLinearProjection;
		lhs.vPosition -= vCorrection * lhs.fInverseMass;
		rhs.vPosition += vCorrection * rhs.fInverseMass;

		vRelativeVelocity = rhs.vVelocity - lhs.vVelocity;
		math::Vector vTangent = math::Vector::Normalize2d(vRelativeVelocity - collisionData.vNormal * (vRelativeVelocity.Dot(collisionData.vNormal)));
		float fFrictionMagnitude = -vTangent.Dot(vRelativeVelocity) / (lhs.fInverseMass + rhs.fInverseMass);
		float fFrictionSq = math::Squared(lhs.fFriction) + math::Squared(rhs.fFriction);
		math::Vector vFriction = abs(fFrictionMagnitude) < fImpulseMagnitude * fFrictionSq ? vTangent * fFrictionMagnitude : vTangent * -fImpulseMagnitude * fFrictionSq;
		lhs.vVelocity -= vFriction * lhs.fInverseMass;
		rhs.vVelocity += vFriction * rhs.fInverseMass;
	}
}