#include "GraphModel.h"

GraphModel::GraphModel(ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	cVertexCount = 25 * 25;

	VertexType *pVertices = new VertexType[cVertexCount];

	for (size_t i = 0; i < 25; i++)
	{
		for (size_t j = 0; j < 25; j++)
		{
			pVertices[i + (j * 25)].position = { (float)i * 0.05f, sinf((float)i * 0.05f * 3.14159f), (float)j * 0.05f, 1.0f};
			pVertices[i + (j * 25)].color = { ((float)i * 0.25f)/6.25f, ((float)j*0.25f)/ 6.25f, sinf((float)i * 0.1f * 3.14159f), 1.0f };
		}
	}

	// (2*m - 2)*n + (2*n - 2)*m
	cIndexCount = (( (2 * 25) - 2) * 25) + (( (2*25) - 2) * 25);
	unsigned long *pIndex = new unsigned long[cIndexCount];	

	memset(pIndex, 0, sizeof(unsigned long) * cIndexCount);
	
	unsigned long posIndex = 0;
	
	for (unsigned long j = 0; j < 25; j++)
	{
		for (unsigned long i = 0; i < 24; i++)
		{
			pIndex[posIndex] = i + (j * 25);
			pIndex[posIndex + 1] = (i + 1) + (j * 25);
	
			posIndex += 2;
		}
	
	}
		
	for (unsigned long i = 0; i < 25; i++)
	{
		for (unsigned long j = 0; j < 24; j++)
		{
			pIndex[posIndex + 1] = i + ((j + 1) * 25);
			pIndex[posIndex] = i + (j * 25);
	
			posIndex += 2;
		}
	}
		
	
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

GraphModel::GraphModel(const GridDirectX& dataPoints, ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	cVertexCount = dataPoints.points.size();

	// (2*m - 2)*n + (2*n - 2)*m
	cIndexCount = (((2 * dataPoints.m) - 2) * dataPoints.n) + (((2 * dataPoints.n) - 2) * dataPoints.m);

	unsigned long *pIndex = new unsigned long[cIndexCount];

	//memset(pIndex, 0, sizeof(unsigned long) * cIndexCount);

	unsigned long posIndex = 0;

	for (unsigned long j = 0; j < dataPoints.m; j++)
	{
		for (unsigned long i = 0; i < dataPoints.n - 1; i++)
		{
			pIndex[posIndex] = i + (j * dataPoints.n);
			pIndex[posIndex + 1] = (i + 1) + (j * dataPoints.n);

			posIndex += 2;
		}

	}

	for (unsigned long i = 0; i < dataPoints.n; i++)
	{
		for (unsigned long j = 0; j < dataPoints.m - 1; j++)
		{
			pIndex[posIndex + 1] = i + ((j + 1) * dataPoints.n);
			pIndex[posIndex] = i + (j * dataPoints.n);

			posIndex += 2;
		}
	}


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * cVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = dataPoints.points.data();
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

GraphModel::~GraphModel()
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

ID3D11Buffer* GraphModel::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* GraphModel::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int GraphModel::GetIndexCount() const
{
	return cIndexCount;
}