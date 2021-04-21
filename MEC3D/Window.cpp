#include "Window.h"

uint32_t Window::cNumJanelas = 0;

Window::WindowClass Window::WindowClass::cWndClass;

Window::WindowClass::WindowClass() noexcept
	:
cHInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = cHInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(cWndClassName, cHInst);
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return cWndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return cWndClass.cHInst;
}

Window::Window(int width, int height, const wchar_t* wndTitle) noexcept
	:
	cAberta(true)
{
	RECT ret;
	ret.left = 100;
	ret.right = ret.left + width;
	ret.top = 50;
	ret.bottom = ret.top + height;

	AdjustWindowRect(&ret, WS_OVERLAPPEDWINDOW, FALSE);

	cHWnd = CreateWindow(
		WindowClass::GetName(), wndTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, ret.right - ret.left, ret.bottom - ret.top, 
		nullptr, nullptr, WindowClass::GetInstance(), this);

	cNumJanelas++;

	ShowWindow(cHWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(cHWnd);

	cNumJanelas--;

	cAberta = false;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsgThunk(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const  pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:		
		PostQuitMessage(0);

		this->~Window();

		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}