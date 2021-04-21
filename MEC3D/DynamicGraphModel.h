#pragma once
#include <vector>
#include "ColorShader.h"
#include "DataPoint.h"

class DynamicGraphModel
{
public:
	DynamicGraphModel(ID3D11Device* pDevice);
	DynamicGraphModel(const GridDirectX& dataPoints, ID3D11Device* pDevice);

	bool ChangePoints(const GridDirectX& dataPoints, DirectX11* pDX11);

	~DynamicGraphModel();
	DynamicGraphModel() = delete;
	DynamicGraphModel(const DynamicGraphModel&) = delete;

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;

	const unsigned int cM, cN;
};