#include "TriangularMeshWindow.h"
#include <string>
#include <iostream>

TriangularMeshWindow::TriangularMeshWindow(int width, int height, const wchar_t* titulo, const std::vector<VertexType>& pontos) noexcept
	:
	GraphWindow(width, height, titulo),
	cPMeshModel(nullptr),
	cPLineMeshModel(nullptr),
	cMostrarWireframe(false)
{
	cPMeshModel = new TriangleListMeshModel(pontos, cDirectX11.GetPtrDevice());
	
	cPLineMeshModel = new LineListMeshModel(pontos, cDirectX11.GetPtrDevice());
}

TriangularMeshWindow::~TriangularMeshWindow()
{
	if (cPMeshModel)
	{
		delete cPMeshModel;
		cPMeshModel = nullptr;
	}

	if (cPLineMeshModel)
	{
		delete cPLineMeshModel;
		cPLineMeshModel = nullptr;
	}
}


void TriangularMeshWindow::Update()
{}

void TriangularMeshWindow::Draw()
{
	cCam.Render();
	DirectX::XMMATRIX viewMatrix3D;
	cCam.GetViewMatrix(viewMatrix3D);

	if(!cMostrarWireframe)
		cDirectX11.RenderTriangleMesh(viewMatrix3D, DirectX::XMMatrixRotationY(cRotGraph), cDirectX11.GetProjectionMatrix(),
			cPMeshModel->GetVertexBuffer(), cPMeshModel->GetIndexBuffer(), cPMeshModel->GetIndexCount());
	else
		cDirectX11.RenderGraphGrid(viewMatrix3D, DirectX::XMMatrixRotationY(cRotGraph), cDirectX11.GetProjectionMatrix(),
			cPLineMeshModel->GetVertexBuffer(), cPLineMeshModel->GetIndexBuffer(), cPLineMeshModel->GetIndexCount());
}

void TriangularMeshWindow::CustomInput(char c)
{
	switch (c)
	{
	case 'L':
		cMostrarWireframe = true;
		break;
	case 'T':
		cMostrarWireframe = false;
		break;
	default:
		break;
	}
}