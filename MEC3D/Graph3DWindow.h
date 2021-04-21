#pragma once
#include <vector>
#include "GraphWindow.h"
#include "DirectX11.h"
#include "GraphModel.h"
#include "Camera3D.h"
#include "DataPoint.h"
#include "DynamicGraphModel.h"

class Graph3DWindow : public GraphWindow
{
public:
	Graph3DWindow(int width, int height, const wchar_t* titulo, const GridDirectX& pontos) noexcept;
	Graph3DWindow(const Graph3DWindow&) = delete;
	Graph3DWindow() = delete;
	~Graph3DWindow();
	
private:
	void Draw() override;
	void Update() override;

private:
	
	DynamicGraphModel* cPGraphModel;
};