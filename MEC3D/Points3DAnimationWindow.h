#pragma once
#include <vector>
#include "GraphWindow.h"
#include "DirectX11.h"
#include "DynamicGraphModel.h"
#include "Camera3D.h"
#include "DataPoint.h"

class Points3DAnimationWindow : public GraphWindow
{
public:
	Points3DAnimationWindow(int width, int height, const wchar_t* titulo, const std::vector<GridDirectX>& grids, double dt) noexcept;
	Points3DAnimationWindow(const Points3DAnimationWindow&) = delete;
	Points3DAnimationWindow() = delete;
	~Points3DAnimationWindow();

private:
	void Draw() override;
	void Update() override;

private:
	const std::wstring cTituloOriginal;

	std::vector<GridDirectX> cGrids;

	DynamicGraphModel* cPGraphModel;

	const double cDeltaTime;

	double cTime;
};