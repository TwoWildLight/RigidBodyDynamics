#pragma once
#include "LightWindows.h"
#include "TwlMath.h"
#include <string>

class Graphics
{
public:
	Graphics(HWND hWnd, int w, int h);
public:
	float GetWidth() const;
	float GetHeight() const;
	void BeginFrame();
	void EndFrame();
public:
	void DrawText(const std::string& text, const math::Vector& vPos);
	void DrawLine(const math::Vector& p1, const math::Vector& p2, const math::Vector vColor);
private:
	HDC hMainDC;
	HDC hBufferDC;
	float fResolutionWidth;
	float fResolutionHeight;
};