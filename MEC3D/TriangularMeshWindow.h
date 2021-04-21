#pragma once
#include <vector>
#include "GraphWindow.h"
#include "DirectX11.h"
#include "GraphModel.h"
#include "Camera3D.h"
#include "DataPoint.h"
#include "TriangleListMeshModel.h"
#include "LineListMeshModel.h"

class TriangularMeshWindow : public GraphWindow
{
public:
	TriangularMeshWindow(int width, int height, const wchar_t* titulo, const std::vector<VertexType>& pontos) noexcept;
	TriangularMeshWindow(const TriangularMeshWindow&) = delete;
	TriangularMeshWindow() = delete;
	~TriangularMeshWindow();

private:
	void Draw() override;
	void Update() override;

	void CustomInput(char c) override;

private:

	TriangleListMeshModel* cPMeshModel;

	LineListMeshModel* cPLineMeshModel;

	bool cMostrarWireframe;
};