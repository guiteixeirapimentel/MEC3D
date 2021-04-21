#include "Graph3DWindow.h"
#include <string>
#include <iostream>

Graph3DWindow::Graph3DWindow(int width, int height, const wchar_t* titulo, const GridDirectX& pontos) noexcept
	:
	GraphWindow(width, height, titulo),
	cPGraphModel(nullptr)
{
	cPGraphModel = new DynamicGraphModel(pontos, cDirectX11.GetPtrDevice());
}

Graph3DWindow::~Graph3DWindow()
{
	if (cPGraphModel)
	{
		delete cPGraphModel;
		cPGraphModel = nullptr;
	}
}


void Graph3DWindow::Update()
{}

void Graph3DWindow::Draw()
{
	cCam.Render();
	DirectX::XMMATRIX viewMatrix3D;
	cCam.GetViewMatrix(viewMatrix3D);
	
	cDirectX11.RenderGraphGrid(viewMatrix3D, DirectX::XMMatrixRotationY(cRotGraph), cDirectX11.GetProjectionMatrix(),
		cPGraphModel->GetVertexBuffer(), cPGraphModel->GetIndexBuffer(), cPGraphModel->GetIndexCount());
}