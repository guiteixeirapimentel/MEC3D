#pragma once
#include <vector>
#include "ColorShader.h"
#include "DataPoint.h"

class TriangleListMeshModel
{
public:
	TriangleListMeshModel(ID3D11Device* pDevice);
	TriangleListMeshModel(const std::vector<VertexType>& points, ID3D11Device* pDevice);

	~TriangleListMeshModel();
	TriangleListMeshModel() = delete;
	TriangleListMeshModel(const TriangleListMeshModel&) = delete;

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer* cPVertexBuffer, * cPIndexBuffer;
	UINT cVertexCount, cIndexCount;
};