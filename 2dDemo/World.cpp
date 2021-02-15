#include "World.h"
#include "Body.h"
#include <algorithm>

void World::AddBody(std::unique_ptr<Body> pBody)
{
	bodyPtrs.push_back(std::move(pBody));
}

void World::Update(float fDeltaTime)
{
	for (auto& pBody : bodyPtrs)
	{
		physics.ApplyPhysics(*pBody);
		pBody->Integrate(fDeltaTime);
	}

	std::erase_if(bodyPtrs,
		[](const std::unique_ptr<Body>& pBody)
		{
			return pBody->GetPosition().y < -350.0f;
		});

	for (size_t i = 0; i < bodyPtrs.size(); i++)
	{
		for (size_t j = 0; j < bodyPtrs.size(); j++)
		{
			if (bodyPtrs[i] != bodyPtrs[j])
			{
				physics.HandleCollision(*bodyPtrs[i], *bodyPtrs[j]);
			}
		}
	}
}

void World::Render(Graphics& gfx)
{
	for (auto& pBody : bodyPtrs)
	{
		const auto& mesh = pBody->GetMesh();
		const auto mTransform =
			math::Matrix::Rotation(pBody->GetAngle()) *
			math::Matrix::Translation(pBody->GetPosition());

		for (size_t i = 0; i < mesh.indices.size(); i += 2)
		{
			const auto vP1 = mesh.vertices[mesh.indices[i]] * mTransform;
			const auto vP2 = mesh.vertices[mesh.indices[i + 1]] * mTransform;
			gfx.DrawLine(vP1, vP2, { 255.0f, 255.0f, 255.0f });
		}
	}
}

void World::Clear()
{
	std::erase_if(bodyPtrs,
		[](const std::unique_ptr<Body>& pBody)
		{
			return pBody->GetType() == Body::Type::btDynamic;
		});
}