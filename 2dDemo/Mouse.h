#pragma once
#include "LightWindows.h"
#include <vector>

namespace twl
{
	class Mouse
	{
	private:
		friend class Window;
		enum class ButtonState
		{
			kFree,
			kDown,
			kHold,
			kUp
		};
		enum class WheelState
		{
			kIdle,
			kForward,
			kBackward
		};
		static constexpr int kNumButtons = 7;
	public:
		static void EnableImGuiCursor();
		static void DisableImGuiCursor();
		static void ShowCursor();
		static void HideCursor();
		static void ConfineCursor();
		static void FreeCursor();
		static bool IsCursorConfined();
		static bool IsButtonFree(int iButton);
		static bool IsButtonDown(int iButton);
		static bool IsButtonHold(int iButton);
		static bool IsButtonUp(int iButton);
		static bool IsWheelForward();
		static bool IsWheelBackward();
		static int GetPosX();
		static int GetPosY();
		static POINT GetPosition();
		static std::vector<POINT>& GetRawDeltas();
	private:
		static void RegisterRawInputDevice(HWND hWnd);
		static bool HandleWM(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static void Update();
	private:
		Mouse() = default;
		static Mouse m;
	private:
		ButtonState previousButtonStates[kNumButtons] = {};
		ButtonState currentButtonStates[kNumButtons] = {};
		WheelState wheelState = WheelState::kIdle;
		std::vector<POINT> rawDeltas[2];
		size_t uActiveRawDeltaIndex = 0;
		POINT ptPosition = {};
		bool bWheelMoved = false;
		bool bConfined = false;
	};
}