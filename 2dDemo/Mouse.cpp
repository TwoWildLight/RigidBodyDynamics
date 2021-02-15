#include "Mouse.h"

Mouse Mouse::m;

void Mouse::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0) {}
}

void Mouse::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0) {}
}

void Mouse::ConfineCursor()
{
	m.bConfined = true;
}

void Mouse::FreeCursor()
{
	m.bConfined = false;
}

bool Mouse::IsCursorConfined()
{
	return m.bConfined;
}

bool Mouse::IsButtonFree(int iButton)
{
	return m.currentButtonStates[iButton] == ButtonState::kFree;
}

bool Mouse::IsButtonDown(int iButton)
{
	return m.currentButtonStates[iButton] == ButtonState::kDown;
}

bool Mouse::IsButtonHold(int iButton)
{
	return m.currentButtonStates[iButton] == ButtonState::kHold;
}

bool Mouse::IsButtonUp(int iButton)
{
	return m.currentButtonStates[iButton] == ButtonState::kUp;
}

bool Mouse::IsWheelForward()
{
	return m.wheelState == WheelState::kForward;
}

bool Mouse::IsWheelBackward()
{
	return m.wheelState == WheelState::kBackward;
}

int Mouse::GetPosX()
{
	return m.ptPosition.x;
}

int Mouse::GetPosY()
{
	return m.ptPosition.y;
}

POINT Mouse::GetPosition()
{
	return m.ptPosition;
}

std::vector<POINT>& Mouse::GetRawDeltas()
{
	return m.rawDeltas[m.uActiveRawDeltaIndex == 1 ? 0 : 1];
}

void Mouse::RegisterRawInputDevice(HWND hWnd)
{
	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.hwndTarget = hWnd;
	RegisterRawInputDevices(&rid, 1u, sizeof(rid));
}

bool Mouse::HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		m.currentButtonStates[VK_LBUTTON] = ButtonState::kDown;
		break;
	case WM_RBUTTONDOWN:
		m.currentButtonStates[VK_RBUTTON] = ButtonState::kDown;
		break;
	case WM_MBUTTONDOWN:
		m.currentButtonStates[VK_MBUTTON] = ButtonState::kDown;
		break;
	case WM_XBUTTONDOWN:
		if (HIWORD(wParam) == 0x0001)
		{
			m.currentButtonStates[VK_XBUTTON1] = ButtonState::kDown;
		}
		else
		{
			m.currentButtonStates[VK_XBUTTON2] = ButtonState::kDown;
		}
		break;
	case WM_LBUTTONUP:
		m.currentButtonStates[VK_LBUTTON] = ButtonState::kUp;
		break;
	case WM_RBUTTONUP:
		m.currentButtonStates[VK_RBUTTON] = ButtonState::kUp;
		break;
	case WM_MBUTTONUP:
		m.currentButtonStates[VK_MBUTTON] = ButtonState::kUp;
		break;
	case WM_XBUTTONUP:
		if (HIWORD(wParam) == 0x0001)
		{
			m.currentButtonStates[VK_XBUTTON1] = ButtonState::kUp;
		}
		else
		{
			m.currentButtonStates[VK_XBUTTON2] = ButtonState::kUp;
		}
		break;
	case WM_MOUSEWHEEL:
	{
		m.wheelState = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? WheelState::kForward : WheelState::kBackward;
		m.bWheelMoved = true;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (m.bConfined)
		{
			RECT rt;
			GetClientRect(hWnd, &rt);
			MapWindowPoints(hWnd, nullptr, (POINT*)&rt, 2);
			ClipCursor(&rt);
		}
		else
		{
			ClipCursor(nullptr);
			POINTS pts = MAKEPOINTS(lParam);
			m.ptPosition.x = pts.x;
			m.ptPosition.y = pts.y;
		}
		break;
	}
	case WM_INPUT:
	{
		UINT size = 0u;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

		std::vector<BYTE> rawInputBuffer(size);
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawInputBuffer.data(), &size, sizeof(RAWINPUTHEADER));

		auto& ri = reinterpret_cast<const RAWINPUT&>(*(rawInputBuffer.data()));
		if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			m.rawDeltas[m.uActiveRawDeltaIndex].push_back({ ri.data.mouse.lLastX, ri.data.mouse.lLastY });
		}
		break;
	}
	default:
		return false;
	}

	return true;
}

void Mouse::Update()
{
	for (int i = 0; i < kNumButtons; i++)
	{
		if (m.previousButtonStates[i] == ButtonState::kDown && m.currentButtonStates[i] == ButtonState::kDown)
		{
			m.currentButtonStates[i] = ButtonState::kHold;
		}
		else if (m.previousButtonStates[i] == ButtonState::kUp && m.currentButtonStates[i] == ButtonState::kUp)
		{
			m.currentButtonStates[i] = ButtonState::kFree;
		}
		m.previousButtonStates[i] = m.currentButtonStates[i];
	}

	if (!m.bWheelMoved)
	{
		m.wheelState = WheelState::kIdle;
	}
	m.bWheelMoved = false;

	m.uActiveRawDeltaIndex = m.uActiveRawDeltaIndex == 1u ? 0u : 1u;
	m.rawDeltas[m.uActiveRawDeltaIndex].clear();
}