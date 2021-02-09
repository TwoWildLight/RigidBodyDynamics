#include "Keyboard.h"

namespace twl
{
	Keyboard Keyboard::kbd;

	bool Keyboard::IsKeyFree(unsigned char key)
	{
		return kbd.currentKeyStates[key] == KeyState::kFree;
	}

	bool Keyboard::IsKeyDown(unsigned char key)
	{
		return kbd.currentKeyStates[key] == KeyState::kDown;
	}

	bool Keyboard::IsKeyHold(unsigned char key)
	{
		return kbd.currentKeyStates[key] == KeyState::kHold;
	}

	bool Keyboard::IsKeyUp(unsigned char key)
	{
		return kbd.currentKeyStates[key] == KeyState::kUp;
	}

	bool Keyboard::IsAutoRepeatEnabled()
	{
		return kbd.bAutoRepeat;
	}

	void Keyboard::EnableAutoRepeat()
	{
		kbd.bAutoRepeat = true;
	}

	void Keyboard::DisableAutoRepeat()
	{
		kbd.bAutoRepeat = false;
	}

	void Keyboard::ClearKeyStates()
	{
		memset(kbd.currentKeyStates, 0, sizeof(kbd.previousKeyStates));
		memset(kbd.currentKeyStates, 0, sizeof(kbd.currentKeyStates));
	}

	bool Keyboard::HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
			if (Keyboard::IsAutoRepeatEnabled() || !(lParam & 0x40000000))
			{
				kbd.currentKeyStates[(unsigned char)wParam] = KeyState::kDown;
			}
			break;
		case WM_SYSKEYUP:
		case WM_KEYUP:
			kbd.currentKeyStates[(unsigned char)wParam] = KeyState::kUp;
			break;
		default:
			return false;
		}

		return true;
	}

	void Keyboard::Update()
	{
		for (int i = 0; i < kNumKeys; i++)
		{
			if (kbd.previousKeyStates[i] == KeyState::kDown && kbd.currentKeyStates[i] == KeyState::kDown)
			{
				kbd.currentKeyStates[i] = KeyState::kHold;
			}
			else if (kbd.previousKeyStates[i] == KeyState::kUp && kbd.currentKeyStates[i] == KeyState::kUp)
			{
				kbd.currentKeyStates[i] = KeyState::kFree;
			}
			kbd.previousKeyStates[i] = kbd.currentKeyStates[i];
		}
	}
}