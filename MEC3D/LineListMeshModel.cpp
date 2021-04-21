#include "LineListMeshModel.h"

LineListMeshModel::LineListMeshModel(ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	cVertexCount = 3;

	VertexType* pVertices = new VertexType[cVertexCount];

	pVertices[0].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	pVertices[0].color = { 1.0f, 0.5f, 0.5f, 1.0f };

	pVertices[1].position = { 1.0f, 0.0f, 0.0f, 1.0f };
	pVertices[1].color = { 1.0f, 0.5f, 0.5f, 1.0f };

	pVertices[2].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	pVertices[2].color = { 1.0f, 0.5f, 0.5f, 1.0f };

	// 3 por triangulo
	cIndexCount = 3;
	unsigned long* pIndex = new unsigned long[cIndexCount];

	memset(pIndex, 0, sizeof(unsigned long) * cIndexCount);

	pIndex[0] = 0;
	pIndex[1] = 2;
	pIndex[2] = 1;


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * cVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = pVertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &cPVertexBuffer);
	if (FAILED(result))
	{
		assert(0);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * cIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = pIndex;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &cPIndexBuffer);
	if (FAILED(result))
	{
		assert(1);
	}

	delete[] pVertices;
	delete[] pIndex;
}

LineListMeshModel::LineListMeshModel(const std::vector<VertexType>& points, ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	std::vector<VertexType> pontosParaPreto;

	pontosParaPreto = points;

	for (size_t i = 0; i < pontosParaPreto.size(); i++)
	{
		pontosParaPreto[i].color = {0.0, 0.0, 0.0, 1.0};
	}

	cVertexCount = pontosParaPreto.size();

	// tres indices por triangulo, tres vertices por triangulo

	cIndexCount = cVertexCount * 2;

	unsigned long* pIndex = new unsigned long[cIndexCount];

	//memset(pIndex, 0, sizeof(unsigned long) * cIndexCount);

	unsigned long indice = 0;

	for (unsigned long j = 0; j < cIndexCount; j+=6)
	{
		pIndex[j] = indice;
		pIndex[j + 1] = indice + 1;

		pIndex[j + 2] = indice + 1;
		pIndex[j + 3] = indice + 2;

		pIndex[j + 4] = indice + 2;
		pIndex[j + 5] = indice;

		indice += 3;
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * cVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = pontosParaPreto.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &cPVertexBuffer);
	if (FAILED(result))
	{
		result = pDevice->GetDeviceRemovedReason();
		MessageBox(NULL, L"Falha ao criar Buffer", L"Erro", 0);
		assert(0);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * cIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = pIndex;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &cPIndexBuffer);
	if (FAILED(result))
	{
		assert(1);
	}

	delete[] pIndex;
}

LineListMeshModel::~LineListMeshModel()
{
	if (cPIndexBuffer)
	{
		cPIndexBuffer->Release();
		cPIndexBuffer = nullptr;
	}

	if (cPVertexBuffer)
	{
		cPVertexBuffer->Release();
		cPVertexBuffer = nullptr;
	}
}

ID3D11Buffer* LineListMeshModel::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* LineListMeshModel::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int LineListMeshModel::GetIndexCount() const
{
	return cIndexCount;
}