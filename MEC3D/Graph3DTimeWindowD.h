#pragma once
#include <vector>
#include "GraphWindow.h"
#include "DirectX11.h"
#include "DynamicGraphModel.h"
#include "Camera3D.h"
#include "DataPoint.h"

class Graph3DTimeWindowD : public GraphWindow
{
public:
	Graph3DTimeWindowD(int width, int height, const wchar_t* titulo, const std::vector<Grid>& grids, double dt) noexcept;
	Graph3DTimeWindowD(int width, int height, const wchar_t* titulo, const std::vector<GridDirectX>& grids, double dt) noexcept;
	Graph3DTimeWindowD(const Graph3DTimeWindowD&) = delete;
	Graph3DTimeWindowD() = delete;
	~Graph3DTimeWindowD();

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