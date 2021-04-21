#include "Shader.h"
#include "DirectX11.h"

Shader::Shader(HWND hWnd, ID3D11Device* pDevice)
	:
	cPPixelShader(nullptr),
	cPVertexShader(nullptr),
	cPLayout(nullptr),
	cPMatrixBuffer(nullptr)
{
}

Shader::~Shader()
{
	// Release the matrix constant buffer.
	if (cPMatrixBuffer)
	{
		cPMatrixBuffer->Release();
		cPMatrixBuffer = nullptr;
	}

	// Release the layout.
	if (cPLayout)
	{
		cPLayout->Release();
		cPLayout = nullptr;
	}

	// Release the pixel shader.
	if (cPPixelShader)
	{
		cPPixelShader->Release();
		cPPixelShader = nullptr;
	}

	// Release the vertex shader.
	if (cPVertexShader)
	{
		cPVertexShader->Release();
		cPVertexShader = nullptr;
	}
}
void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

}