#pragma once
#include <Windows.h>
#include <cstdint>

class Window
{
private:
	class WindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static constexpr const wchar_t* cWndClassName = L"3D Visualiser";
		static WindowClass cWndClass;
		HINSTANCE cHInst;
	};

public:
	Window(int width, int height, const wchar_t* nome) noexcept;
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	
protected:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	static uint32_t PegarNumJanelas()
	{
		return cNumJanelas;
	}

	bool Aberta() const
	{
		return cAberta;
	}
	 
	HWND PegarHandle() const
	{
		return cHWnd;
	}
protected:
	int cWidth;
	int cHeight;
	HWND cHWnd;

	bool cAberta;

	static uint32_t cNumJanelas;
};