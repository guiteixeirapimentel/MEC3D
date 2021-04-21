#include "Graph3DTimeWindowD.h"
#include <string>
#include <iostream>

Graph3DTimeWindowD::Graph3DTimeWindowD(int width, int height, const wchar_t* titulo, const std::vector<Grid>& grids, double dt) noexcept
	:
GraphWindow(width, height, titulo),
cDeltaTime(dt),
cTime(0.0),
cTituloOriginal(titulo),
cPGraphModel(nullptr)
{
	cGrids.resize(grids.size());
	for (size_t i = 0; i < grids.size(); i++)
	{
		cGrids[i] = grids[i];
	}

	cPGraphModel = new DynamicGraphModel(cGrids[0], cDirectX11.GetPtrDevice());
}

Graph3DTimeWindowD::Graph3DTimeWindowD(int width, int height, const wchar_t* titulo, const std::vector<GridDirectX>& grids, double dt) noexcept
	:
GraphWindow(width, height, titulo),
cDeltaTime(dt),
cTime(0.0),
cTituloOriginal(titulo),
cPGraphModel(nullptr)
{
	cGrids = grids;	

	cPGraphModel = new DynamicGraphModel(cGrids[0], cDirectX11.GetPtrDevice());
}

Graph3DTimeWindowD::~Graph3DTimeWindowD()
{
	if (cPGraphModel)
	{
		delete cPGraphModel;
		cPGraphModel = nullptr;
	}
}


void Graph3DTimeWindowD::Update()
{
	cTime += cDeltaTime;
	
	size_t i = static_cast<size_t>(cTime);

	std::wstring iStr = cTituloOriginal + L" n: " + std::to_wstring(i);
	SetWindowText(cHWnd, iStr.c_str());

	if (i >= cGrids.size())
	{
		cTime = 0.0;
		i = static_cast<size_t>(cTime);
	}

	cPGraphModel->ChangePoints(cGrids[i], &cDirectX11);
}

void Graph3DTimeWindowD::Draw()
{
	cCam.Render();
	DirectX::XMMATRIX viewMatrix3D;
	cCam.GetViewMatrix(viewMatrix3D);		

	cDirectX11.RenderGraphGrid(viewMatrix3D, DirectX::XMMatrixRotationY(cRotGraph), cDirectX11.GetProjectionMatrix(),
		cPGraphModel->GetVertexBuffer(), cPGraphModel->GetIndexBuffer(), cPGraphModel->GetIndexCount());
}