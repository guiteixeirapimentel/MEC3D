#pragma once
#include <vector>
#include "ColorShader.h"
#include "DataPoint.h"

class LineListMeshModel
{
public:
	LineListMeshModel(ID3D11Device* pDevice);
	LineListMeshModel(const std::vector<VertexType>& points, ID3D11Device* pDevice);

	~LineListMeshModel();
	LineListMeshModel() = delete;
	LineListMeshModel(const LineListMeshModel&) = delete;

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer* cPVertexBuffer, * cPIndexBuffer;
	UINT cVertexCount, cIndexCount;
};