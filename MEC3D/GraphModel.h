#pragma once
#include <vector>
#include "ColorShader.h"
#include "DataPoint.h"

class GraphModel
{
public:
	GraphModel(ID3D11Device* pDevice);
	GraphModel(const GridDirectX& dataPoints, ID3D11Device* pDevice);

	~GraphModel();
	GraphModel() = delete;
	GraphModel(const GraphModel&) = delete;

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	UINT cVertexCount, cIndexCount;
};