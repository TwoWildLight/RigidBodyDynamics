#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

std::string Window::className = "TwlWindowClass";

Window::Window(const std::string& titleName, int w, int h)
	:
	titleName(titleName)
{
	HINSTANCE hInstance = GetModuleHandleA(nullptr);
	RegisterWindowClass(hInstance);
	CreateAndShowWindow(hInstance, w, h);

	Mouse::RegisterRawInputDevice(hWnd);

	pGraphics = std::make_unique<Graphics>(hWnd, w, h);
}

Window::~Window()
{
	DestroyWindow(hWnd);
	UnregisterClassA(className.c_str(), nullptr);
}

bool Window::ProceedMessage() const
{
	MSG msg = {};
	while (PeekMessageA(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	Keyboard::Update();
	Mouse::Update();

	return true;
}

void Window::Kill() const
{
	PostQuitMessage(S_OK);
}

Graphics& Window::GetGraphics()
{
	return *pGraphics;
}

void Window::RegisterWindowClass(HINSTANCE hInstance) const
{
	WNDCLASSEXA wc = {};
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(APP_ICON), IMAGE_ICON, 48u, 48u, 0u);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = className.c_str();
	RegisterClassExA(&wc);
}

void Window::CreateAndShowWindow(HINSTANCE hInstance, int w, int h)
{
	RECT rtWindow = { 0, 0, w, h };
	AdjustWindowRect(&rtWindow, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	hWnd = CreateWindowA(
		className.c_str(), titleName.c_str(),
		WS_SYSMENU | WS_MINIMIZEBOX,
		GetSystemMetrics(SM_CXSCREEN) / 2 - (rtWindow.right - rtWindow.left) / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - (rtWindow.bottom - rtWindow.top) / 2,
		rtWindow.right - rtWindow.left, rtWindow.bottom - rtWindow.top,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Keyboard::HandleWM(hWnd, uMsg, wParam, lParam)
		|| Mouse::HandleWM(hWnd, uMsg, wParam, lParam))
	{
		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(S_OK);
		return S_OK;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}