#pragma once
#include <vector>
#include "GraphWindow.h"
#include "DirectX11.h"
#include "GraphModel.h"
#include "Camera3D.h"
#include "DataPoint.h"

class Graph3DTimeWindow : public GraphWindow
{
public:
	Graph3DTimeWindow(int width, int height, const wchar_t* titulo, const std::vector<Grid>& grids, double dt) noexcept;
	Graph3DTimeWindow(const Graph3DTimeWindow&) = delete;
	Graph3DTimeWindow() = delete;
	~Graph3DTimeWindow();
	
private:
	void Draw();
	void Update();


private:	
	const std::wstring cTituloOriginal;

	std::vector<GraphModel*> cPGraphsModel;

	const double cDeltaTime;

	double cTime;
};