#pragma once
#include "Window.h"
#include "DirectX11.h"
#include "Camera3D.h"
#include "Keyboard.h"
#include "Mouse.h"

class GraphWindow : public Window
{
public:
	GraphWindow(int width, int height, const wchar_t* titulo) noexcept;
	GraphWindow(const GraphWindow&) = delete;
	GraphWindow() = delete;
	~GraphWindow();

	void Tick();

protected:
	virtual void Draw() = 0;
	virtual void Update() = 0;

	virtual void UpdateCamera();

	virtual void Input();

	virtual void CustomInput(char key) {};

protected:
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;
	
private:
	MouseServer cMouseServer;
	KeyboardServer cKeybdServer;

protected:
	DirectX11 cDirectX11;
	Camera3D cCam;

	MouseClient cMouse;
	KeyboardClient cKeyboard;

	DirectX::XMFLOAT2 cUltPosMouseBotEsqPress;
	bool cBotEsqPress;

	float cRotGraph;
	DirectX::XMVECTOR cPosCamera;
	DirectX::XMVECTOR cLookAtCamera;

	float cAngVert;
	float cAngHoriz;
};