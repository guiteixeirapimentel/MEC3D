#include "Simple2DWindow.h"

#include <windowsx.h>

Simple2DWindow::Simple2DWindow(int width, int height, const wchar_t* titulo) noexcept
	:
	Window(width, height, titulo),
	cMouse(cMouseServer),
	cKeyboard(cKeybdServer)
{}

Simple2DWindow::~Simple2DWindow()
{}

LRESULT Simple2DWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		//Graph3DWindow::~Graph3DWindow();
		cAberta = false;
		cNumJanelas--;
		break;
	case WM_SIZE:
	{
		RECT r = {};
		GetClientRect(hWnd, &r);

		LONG cWndWidth = r.right - r.left;
		LONG cWndHeight = r.bottom - r.top;

		cWidth = cWndWidth;
		cHeight = cWndHeight;
	}
	break;
	case WM_KEYDOWN:
		cKeybdServer.OnKeyPressed((unsigned char)wParam);
		break;
	case WM_KEYUP:
		cKeybdServer.OnKeyReleased((unsigned char)wParam);
		break;
	case WM_CHAR:
		cKeybdServer.OnChar((unsigned char)wParam);
		break;
	case WM_MOUSEMOVE:
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);
		if (x > 0 && x < cWidth && y > 0
			&& y < cHeight)
		{
			cMouseServer.OnMouseMove(x, y);
			if (!cMouseServer.IsInWindow())
			{
				SetCapture(hWnd);
				cMouseServer.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				x = max(0, x);
				x = min(cWidth - 1, x);
				y = max(0, y);
				y = min(cHeight - 1, y);
				cMouseServer.OnMouseMove(x, y);
			}
			else
			{
				ReleaseCapture();
				cMouseServer.OnMouseLeave();
				cMouseServer.OnLeftReleased(x, y);
				cMouseServer.OnRightReleased(x, y);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);
		cMouseServer.OnLeftPressed(x, y);
		SetFocus(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);
		cMouseServer.OnRightPressed(x, y);
		SetFocus(hWnd);
		break;
	}
	case WM_LBUTTONUP:
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);
		cMouseServer.OnLeftReleased(x, y);
		break;
	}
	case WM_RBUTTONUP:
	{
		int x = (short)LOWORD(lParam);
		int y = (short)HIWORD(lParam);
		cMouseServer.OnRightReleased(x, y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int x = (short)GET_X_LPARAM(lParam);
		int y = (short)GET_Y_LPARAM(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			cMouseServer.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			cMouseServer.OnWheelDown(x, y);
		}
		break;
	}
	case WM_MBUTTONDOWN:
	{
		cMouseServer.OnWheelPress();
		break;
	}
	case WM_MBUTTONUP:
	{
		cMouseServer.OnWheelRelease();
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Simple2DWindow::Tick()
{
	if (cAberta)
	{
		Draw();

		Input();
		Update();
	}
}

void Simple2DWindow::Input()
{
	while (!cMouse.MouseEmpty())
	{
		MouseEvent me = cMouse.ReadMouse();

		switch (me.GetType())
		{
		case MouseEvent::WheelUp:
		{
		}
		break;
		case MouseEvent::WheelDown:
		{
		}
		break;
		}

	}

	if (cMouse.LeftIsPressed())
	{
		
	}
	else
	{
	}
}