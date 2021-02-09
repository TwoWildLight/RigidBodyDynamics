#include "Demo.h"
#include "Mouse.h"

twl::Demo::Demo()
	:
	Window("demo", 1024, 768)
{
}

void twl::Demo::Initiate()
{
	while (Window::ProceedMessage())
	{
		GetGraphics().BeginFrame();
		GetGraphics().DrawText(std::to_string(1.0f / timer.Peek()), { 0.0f, 0.0f });
		Update(timer.Mark());
		Render();
		GetGraphics().EndFrame();
	}
}

void twl::Demo::Update(float fDeltaTime)
{

}

void twl::Demo::Render()
{
	GetGraphics().DrawLine({ -400.0f, 300.0f }, { 400.0f, 300.0f }, { 255.0f, 55.0f, 83 });
	GetGraphics().DrawLine({ -400.0f, -300.0f }, { 400.0f, -300.0f }, { 255.0f, 55.0f, 83 });
	GetGraphics().DrawLine({ -400.0f, 300.0f }, { -400.0f, -300.0f }, { 255.0f, 55.0f, 83 });
	GetGraphics().DrawLine({ 400.0f, 300.0f }, { 400.0f, -300.0f }, { 255.0f, 55.0f, 83 });
}