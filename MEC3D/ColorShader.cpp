#include "ColorShader.h"
#include "DirectX11.h"

ColorShader::ColorShader(HWND hWnd, DirectX11& gfx)
	:
	Shader(hWnd, gfx.GetPtrDevice())
{
	HRESULT result;

	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(L"Color.vs", NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, NULL, L"Color.vs");
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(NULL, L"Color.vs", L"Missing Shader File", MB_OK);
		}

		MessageBox(NULL, L"Erro", L"Erro", 0);
	}


	ID3D10Blob* pixelShaderBuffer = nullptr;

	// Compile the pixel shader code.
	result = D3DCompileFromFile(L"Color.ps", NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, NULL, L"Color.ps");
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(NULL, L"Color.ps", L"Missing Shader File", MB_OK);
		}

		MessageBox(NULL, L"Erro", L"Erro", 0);
	}

	// Create the vertex shader from the buffer.
	result = gfx.GetPtrDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &cPVertexShader);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Erro", L"Erro", 0);
	}

	// Create the pixel shader from the buffer.
	result = gfx.GetPtrDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &cPPixelShader);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Erro", L"Erro", 0);
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = gfx.GetPtrDevice()->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &cPLayout);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Erro", L"Erro", 0);
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	D3D11_BUFFER_DESC matrixBufferDesc;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = gfx.GetPtrDevice()->CreateBuffer(&matrixBufferDesc, NULL, &cPMatrixBuffer);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Erro", L"Erro", 0);
	}
}

ColorShader::~ColorShader()
{

}