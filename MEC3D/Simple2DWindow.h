#pragma once
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"

class Simple2DWindow : public Window
{
public:
	struct SteadyFieldInfo2D
	{
		std::vector<float> cUVelField;
		std::vector<float> cVVelField;
		std::vector<float> cWVelField;

		std::vector<float> cPressField;

		int32_t cNX;
		int32_t cNZ;

		float cdx;
		float cdz;
	};

	struct UnsteadyFieldInfo2D
	{
		std::vector<SteadyFieldInfo2D> cFields;

		int32_t cNT;
		float dt;
	};
public:
	Simple2DWindow(int width, int height, const wchar_t* titulo) noexcept;
	Simple2DWindow(const Simple2DWindow&) = delete;
	Simple2DWindow() = delete;
	~Simple2DWindow();

	void Tick();

protected:
	virtual void Draw() = 0;
	virtual void Update() = 0;
	
	virtual void Input();

protected:
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept override;

private:
	MouseServer cMouseServer;
	KeyboardServer cKeybdServer;

protected:
	MouseClient cMouse;
	KeyboardClient cKeyboard;	
};