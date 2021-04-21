#pragma once
#include "Shader.h"
#include <DirectXMath.h>

struct VertexType
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};

struct MatrixBufferType
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX orthoMatrix;
};

class ColorShader : public Shader
{
public:
	ColorShader(HWND hWnd, DirectX11& gfx);
	~ColorShader();

};