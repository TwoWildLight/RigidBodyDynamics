#pragma once
#include "LightWindows.h"
#include <memory>
#include <string>

#include "Graphics.h"

class Window
{
public:
	Window(const std::string& titleName, int w, int h);
	virtual ~Window();
protected:
	virtual void Initiate() abstract;
	bool ProceedMessage() const;
	void Kill() const;
	Graphics& GetGraphics();
private:
	void RegisterWindowClass(HINSTANCE hInstance) const;
	void CreateAndShowWindow(HINSTANCE hInstance, int w, int h);
	static LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	static std::string className;
	std::string titleName;
	HWND hWnd;
private:
	std::unique_ptr<Graphics> pGraphics;
};