#include "Graph3DTimeWindow.h"
#include <string>
#include <iostream>

Graph3DTimeWindow::Graph3DTimeWindow(int width, int height, const wchar_t* titulo, const std::vector<Grid>& grids, double dt) noexcept
		:
		GraphWindow(width, height, titulo),
		cDeltaTime(dt),
		cTime(0.0),
		cTituloOriginal(titulo)
{
	cPGraphsModel.resize(grids.size(), nullptr);

	for (size_t i = 0; i < grids.size(); i++)
	{
		GridDirectX gd;
		gd = grids[1];
		cPGraphsModel[i] = new GraphModel(gd, cDirectX11.GetPtrDevice());
	}
}

Graph3DTimeWindow::~Graph3DTimeWindow()
{
	for (size_t i = 0; i < cPGraphsModel.size(); i++)
	{
		if (cPGraphsModel[i])
		{
			delete cPGraphsModel[i];
			cPGraphsModel[i] = nullptr;
		}
	}
	cPGraphsModel.clear();
}


void Graph3DTimeWindow::Update()
{
	cTime += cDeltaTime;
}

void Graph3DTimeWindow::Draw()
{
	cCam.Render();
	DirectX::XMMATRIX viewMatrix3D;
	cCam.GetViewMatrix(viewMatrix3D);
	
	size_t i = static_cast<size_t>(cTime);

	std::wstring iStr = cTituloOriginal + L" n: " + std::to_wstring(i);
	SetWindowText(cHWnd, iStr.c_str());

	if (i >= cPGraphsModel.size())
	{
		cTime = 0.0;
		i = static_cast<size_t>(cTime);
	}

	cDirectX11.RenderGraphGrid(viewMatrix3D, DirectX::XMMatrixRotationY(cRotGraph), cDirectX11.GetProjectionMatrix(),
		cPGraphsModel[i]->GetVertexBuffer(), cPGraphsModel[i]->GetIndexBuffer(), cPGraphsModel[i]->GetIndexCount());
}