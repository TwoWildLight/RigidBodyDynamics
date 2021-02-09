#pragma once
#include "LightWindows.h"

namespace twl
{
	class Keyboard final
	{
		friend class Window;
	private:
		enum class KeyState
		{
			kFree,
			kDown,
			kHold,
			kUp
		};
		static constexpr int kNumKeys = 256;
	public:
		static bool IsKeyFree(unsigned char key);
		static bool IsKeyDown(unsigned char key);
		static bool IsKeyHold(unsigned char key);
		static bool IsKeyUp(unsigned char key);

		static bool IsAutoRepeatEnabled();
		static void EnableAutoRepeat();
		static void DisableAutoRepeat();
		static void ClearKeyStates();
	private:
		Keyboard() = default;
		static Keyboard kbd;
	private:
		static bool HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static void Update();
	private:
		KeyState previousKeyStates[kNumKeys] = {};
		KeyState currentKeyStates[kNumKeys] = {};
		bool bAutoRepeat = false;
	};
}