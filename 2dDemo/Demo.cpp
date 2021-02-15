#include "Demo.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <random>
#include <numbers>

Demo::Demo()
	:
	Window("demo", 1024, 768)
{
	math::Mesh mesh;
	mesh.vertices.push_back({ -300.0f, 10.0f });
	mesh.vertices.push_back({ 300.0f, 10.0f });
	mesh.vertices.push_back({ 300.0f, -10.0f });
	mesh.vertices.push_back({ -300.0f, -10.0f });
	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);
	mesh.indices.push_back(3);
	mesh.indices.push_back(0);
	world.AddBody(std::make_unique<Body>(Body::Type::btStatic, mesh, math::Vector(0.0f, -275.0f), math::Vector(), 0.0f, 0.2f, 0.1f, 0.5f));

	mesh.vertices[0] = math::Vector(-50.0f, 50.0f);
	mesh.vertices[1] = math::Vector(50.0f, 50.0f);
	mesh.vertices[2] = math::Vector(50.0f, -50.0f);
	mesh.vertices[3] = math::Vector(-50.0f, -50.0f);
	world.AddBody(std::make_unique<Body>(Body::Type::btDynamic, mesh, math::Vector(), math::Vector(), 1.0f, 0.2f, 0.1f, 0.5f));
	world.AddBody(std::make_unique<Body>(Body::Type::btDynamic, mesh, math::Vector(0.0f, 110.0f), math::Vector(), 1.0f, 0.2f, 0.1f, 0.5f));
	world.AddBody(std::make_unique<Body>(Body::Type::btDynamic, mesh, math::Vector(0.0f, 220.0f), math::Vector(), 1.0f, 0.2f, 0.1f, 0.5f));
	world.AddBody(std::make_unique<Body>(Body::Type::btDynamic, mesh, math::Vector(0.0f, 330.0f), math::Vector(), 1.0f, 0.2f, 0.1f, 0.5f));
}

void Demo::Initiate()
{
	while (Window::ProceedMessage())
	{
		GetGraphics().BeginFrame();
		Update(timer.Mark());
		Render();
		GetGraphics().EndFrame();
	}
}

void Demo::Update(float fDeltaTime)
{
	SpawnBody();
	world.Update(fDeltaTime);

	if (Keyboard::IsKeyDown(VK_BACK))
	{
		world.Clear();
	}
}

void Demo::Render()
{
	world.Render(GetGraphics());
}

void Demo::SpawnBody()
{
	static std::random_device rd;
	static std::mt19937 rng(rd());
	static std::uniform_int_distribution<uint32_t> numVertices(5, 5);
	static constexpr float fRadius = 45.0f;

	if (Mouse::IsButtonDown(VK_LBUTTON))
	{
		math::Mesh mesh;

		uint32_t uNumVertices = numVertices(rng);
		for (uint32_t i = 0; i < uNumVertices; i++)
		{
			float fAngle = (float)std::numbers::pi * 2.0f / (float)uNumVertices * (float)i;
			mesh.vertices.push_back(math::Vector(fRadius) * math::Matrix::Rotation(0.0f, 0.0f, fAngle));
		}

		mesh.indices.push_back(0);
		for (uint32_t i = 0; i < uNumVertices - 1; i++)
		{
			mesh.indices.push_back(i + 1);
			mesh.indices.push_back(i + 1);
		}
		mesh.indices.push_back(0);

		math::Vector vMouse((float)Mouse::GetPosX(), -(float)Mouse::GetPosY());
		vMouse.x -= GetGraphics().GetWidth() * 0.5f;
		vMouse.y += GetGraphics().GetHeight() * 0.5f;

		world.AddBody(std::make_unique<Body>(Body::Type::btDynamic, mesh, vMouse, math::Vector(0.0f, 0.0f, std::numbers::pi * 0.5f), 1.0f, 0.2f, 0.1f, 0.5f));
	}
}