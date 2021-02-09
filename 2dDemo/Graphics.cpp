#include "Graphics.h"
#include <utility>

namespace twl
{
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
		TextOutA(hBufferDC, vPos.X(), vPos.Y(), text.c_str(), (int)text.length());
	}

	void Graphics::DrawLine(const math::Vector& p1, const math::Vector& p2, const math::Vector vColor)
	{
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(vColor.X(), vColor.Y(), vColor.Z()));
		SelectObject(hBufferDC, hPen);

		MoveToEx(hBufferDC, p1.X() + fResolutionWidth * 0.5f, -p1.Y() + fResolutionHeight * 0.5f, nullptr);
		LineTo(hBufferDC, p2.X() + fResolutionWidth * 0.5f, -p2.Y() + fResolutionHeight * 0.5f);
		DeleteObject(hPen);
	}
}