#include "GraphWindow.h"
#include <string>
#include <iostream>
#include <windowsx.h>

GraphWindow::GraphWindow(int width, int height, const wchar_t* titulo) noexcept
	:
Window(width, height, titulo),
cDirectX11(cHWnd, false),
cMouse(cMouseServer),
cKeyboard(cKeybdServer),
cRotGraph(0.0f),
cAngHoriz(0.0f),
cAngVert(1e-6f)
{
	cPosCamera = { 0.0f, 0.0f, -5.0f, 0.0f };

	cCam.SetPosition(cPosCamera.m128_f32[0], cPosCamera.m128_f32[1], cPosCamera.m128_f32[2]);
	
	cLookAtCamera = { 0.0, 0.0, 1.0, 0.0 };

	cCam.LookAt(cLookAtCamera.m128_f32[0], cLookAtCamera.m128_f32[1], cLookAtCamera.m128_f32[2]);
}

GraphWindow::~GraphWindow()
{}

LRESULT GraphWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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
		if (cWndWidth > 0 && cWndHeight > 0)
			cDirectX11.OnWindowResize();
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
		if (x > 0 && x < static_cast<int>(cDirectX11.GetWindowWidth()) && y > 0
			&& y < static_cast<int>(cDirectX11.GetWindowHeight()))
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
				x = min(static_cast<int>(cDirectX11.GetWindowWidth()) - 1, x);
				y = max(0, y);
				y = min(static_cast<int>(cDirectX11.GetWindowHeight()) - 1, y);
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

void GraphWindow::Tick()
{
	if (cAberta)
	{
		cDirectX11.OpenScene();
		Draw();
		cDirectX11.CloseScene();

		Input();
		Update();
		UpdateCamera();
	}
}

void GraphWindow::Input()
{	
	while (!cMouse.MouseEmpty())
	{
		MouseEvent me = cMouse.ReadMouse();

		switch (me.GetType())
		{
		case MouseEvent::WheelUp:
		{
			cPosCamera = DirectX::XMVectorAdd(cPosCamera,
				DirectX::XMVectorScale(DirectX::XMVector3Normalize(cPosCamera), -0.1f));
		}
			break;
		case MouseEvent::WheelDown:
		{
			cPosCamera = DirectX::XMVectorAdd(cPosCamera,
				DirectX::XMVectorScale(DirectX::XMVector3Normalize(cPosCamera), 0.1f));
		}
			break;
		}

	}

	if (cMouse.LeftIsPressed())
	{
		cBotEsqPress = true;

		DirectX::XMFLOAT2 posMouse = { (float)cMouse.GetMouseX(), (float)cMouse.GetMouseY() };

		//cRotGraph += (cUltPosMouseBotEsqPress.x - posMouse.x) / 300.0f;
		
		//cRotGraph += (cUltPosMouseBotEsqPress.x - posMouse.x) / 300.0f;

		cAngHoriz += (cUltPosMouseBotEsqPress.x - posMouse.x) / 1000.0f;
		cAngVert += (cUltPosMouseBotEsqPress.y - posMouse.y) / 1000.0f;

		//cPosCamera.m128_f32[1] += -(cUltPosMouseBotEsqPress.y - posMouse.y) / 100.0f;

		cUltPosMouseBotEsqPress = posMouse;
	}
	else
	{
		cBotEsqPress = false;
		cUltPosMouseBotEsqPress = { (float)cMouse.GetMouseX(), (float)cMouse.GetMouseY() };
	}

	if (cKeyboard.KeyIsPressed('A'))
	{
		DirectX::XMMATRIX rot90Y = DirectX::XMMatrixRotationX(DirectX::XM_PI/2.0f);

		DirectX::XMVECTOR azimut = DirectX::XMVector3Transform(cLookAtCamera, rot90Y);

		DirectX::XMVECTOR perpLookAt;

		perpLookAt = DirectX::XMVector3Cross(cLookAtCamera, azimut);

		cPosCamera.m128_f32[0] = cPosCamera.m128_f32[0] - perpLookAt.m128_f32[0]*0.1f;
		cPosCamera.m128_f32[1] = cPosCamera.m128_f32[1] - perpLookAt.m128_f32[1]*0.1f;
		cPosCamera.m128_f32[2] = cPosCamera.m128_f32[2] - perpLookAt.m128_f32[2]*0.1f;
	}
	if (cKeyboard.KeyIsPressed('D'))
	{
		DirectX::XMMATRIX rot90Y = DirectX::XMMatrixRotationX(DirectX::XM_PI / 2.0f);

		DirectX::XMVECTOR azimut = DirectX::XMVector3Transform(cLookAtCamera, rot90Y);

		DirectX::XMVECTOR perpLookAt;

		perpLookAt = DirectX::XMVector3Cross(cLookAtCamera, azimut);

		cPosCamera.m128_f32[0] = cPosCamera.m128_f32[0] + perpLookAt.m128_f32[0] * 0.1f;
		cPosCamera.m128_f32[1] = cPosCamera.m128_f32[1] + perpLookAt.m128_f32[1] * 0.1f;
		cPosCamera.m128_f32[2] = cPosCamera.m128_f32[2] + perpLookAt.m128_f32[2] * 0.1f;
	}

	if (cKeyboard.KeyIsPressed('W'))
	{
		//DirectX::XMVECTOR lookAt = { 0.0, 0.0 , 1.0, 0 }/*DirectX::XMVectorNegate(DirectX::XMVector3Normalize(cPosCamera))*/;
		
		cPosCamera.m128_f32[0] = cPosCamera.m128_f32[0] + cLookAtCamera.m128_f32[0] * 0.1;
		cPosCamera.m128_f32[1] = cPosCamera.m128_f32[1] + cLookAtCamera.m128_f32[1] * 0.1;
		cPosCamera.m128_f32[2] = cPosCamera.m128_f32[2] + cLookAtCamera.m128_f32[2] * 0.1;
	}
	if (cKeyboard.KeyIsPressed('S'))
	{
		cPosCamera.m128_f32[0] = cPosCamera.m128_f32[0] - cLookAtCamera.m128_f32[0] * 0.1;
		cPosCamera.m128_f32[1] = cPosCamera.m128_f32[1] - cLookAtCamera.m128_f32[1] * 0.1;
		cPosCamera.m128_f32[2] = cPosCamera.m128_f32[2] - cLookAtCamera.m128_f32[2] * 0.1;
	}

	char c = 0;
	while (c = cKeyboard.ReadKey())
	{
		CustomInput(c);
	}
}

void GraphWindow::UpdateCamera()
{
	cCam.SetPosition(cPosCamera.m128_f32[0]-0.5f, cPosCamera.m128_f32[1] - 0.5f, cPosCamera.m128_f32[2]);

	//DirectX::XMVECTOR lookAt = { 0.0, 0.0 , 1.0, 0 }/*DirectX::XMVectorNegate(DirectX::XMVector3Normalize(cPosCamera))*/;
	
	DirectX::XMMATRIX  rotYHoriz = DirectX::XMMatrixRotationAxis({0.0, 1.0, 0.0, 0.0}, cAngHoriz);

	DirectX::XMMATRIX  rotXVert = DirectX::XMMatrixRotationAxis({ 1.0, 0.0, 0.0, 0.0 }, cAngVert);

	cLookAtCamera = DirectX::XMVector3Transform(cLookAtCamera, rotYHoriz);

	cLookAtCamera = DirectX::XMVector3Transform(cLookAtCamera, rotXVert);

	cAngHoriz = 0.0f;
	cAngVert = 0.0f;

	cCam.LookAt(cLookAtCamera.m128_f32[0], cLookAtCamera.m128_f32[1], cLookAtCamera.m128_f32[2]);
}