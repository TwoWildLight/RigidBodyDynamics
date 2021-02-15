#include "Graphics.h"
#include <utility>

Graphics::Graphics(HWND hWnd, int w, int h)
	:
	fResolutionWidth((float)w),
	fResolutionHeight((float)h)
{
	hMainDC = GetDC(hWnd);
	hBufferDC = CreateCompatibleDC(hMainDC);
	std::vector<uint32_t> colors(size_t(w) * h);
	HBITMAP hBmp = CreateBitmap(w, h, 1u, 32u, colors.data());
	SelectObject(hBufferDC, hBmp);
	DeleteObject(hBmp);
	SetBkColor(hBufferDC, TRANSPARENT);
	SetTextColor(hBufferDC, RGB(255, 255, 255));
}

float Graphics::GetWidth() const
{
	return fResolutionWidth;
}

float Graphics::GetHeight() const
{
	return fResolutionHeight;
}

void Graphics::BeginFrame()
{
	BitBlt(hBufferDC, 0, 0, (int)fResolutionWidth, (int)fResolutionHeight, nullptr, 0, 0, 0x00000042);
}

void Graphics::EndFrame()
{
	BitBlt(hMainDC, 0, 0, (int)fResolutionWidth, (int)fResolutionHeight, hBufferDC, 0, 0, 0x00CC0020);
}

void Graphics::DrawText(const std::string& text, const math::Vector& vPos)
{
	TextOutA(hBufferDC, (int)vPos.x, (int)vPos.y, text.c_str(), (int)text.length());
}

void Graphics::DrawLine(const math::Vector& p1, const math::Vector& p2, const math::Vector vColor)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(vColor.x, vColor.y, vColor.z));
	SelectObject(hBufferDC, hPen);

	MoveToEx(hBufferDC, int(p1.x + fResolutionWidth * 0.5f), int(-p1.y + fResolutionHeight * 0.5f), nullptr);
	LineTo(hBufferDC, int(p2.x + fResolutionWidth * 0.5f), int(-p2.y + fResolutionHeight * 0.5f));
	DeleteObject(hPen);
}