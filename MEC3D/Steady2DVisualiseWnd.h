#pragma once
#include "Simple2DWindow.h"
#include <string>
#include "GradientColorPalette.hpp"

class Steady2DVisualiseWnd : public Simple2DWindow
{
public:
	Steady2DVisualiseWnd(const Simple2DWindow::SteadyFieldInfo2D& field, 
		int viewportWidth, int viewportHeight, float dxViewport, float dyViewport, const wchar_t* titulo);
	Steady2DVisualiseWnd(Simple2DWindow::SteadyFieldInfo2D&& field, 
		int viewportWidth, int viewportHeight, float dxViewport, float dyViewport, const wchar_t* titulo);

	Steady2DVisualiseWnd(const std::string& datBaseFilename, int64_t jValue, float dx, float dz, 
		int viewportWidth, int viewportHeight, float dxViewport, float dyViewport, const wchar_t* titulo);
	Steady2DVisualiseWnd(const Steady2DVisualiseWnd&) = delete;

	Steady2DVisualiseWnd() = delete;
	~Steady2DVisualiseWnd();

public:
	void CutField(int xOffset, int yOffset, int newWidth, int newHeight);

	int GetFieldNX() const
	{
		return cField.cNX;
	}
	int GetFieldNZ() const
	{
		return cField.cNZ;
	}
		
private:
	void Draw() override;
	void Update() override;	

	void Input() override;

private:
	void PutPixel(int x, int y, int cor);

	void SetBufferToU();
	void SetBufferToV();
	void SetBufferToW();
	void SetBufferToP();

	void SetBufferToVel();

	void SetBufferToCurl();

private:
	Simple2DWindow::SteadyFieldInfo2D cField;

	char* cPViewportBuffer;
	int32_t cNXViewport;
	int32_t cNYViewport;
	float cdxViewport;
	float cdyViewport;

	int cShowing = 0;
	bool cNeedsUpdate = true;

	GradientColorPalette cPalette;
};