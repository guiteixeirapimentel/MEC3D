#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "ColorShader.h"

class DirectX11
{
public:
	DirectX11(HWND hWnd, bool cull_back=true);
	DirectX11(const DirectX11&) = delete;
	DirectX11() = delete;
	~DirectX11();

	void OpenScene();
	void CloseScene();

	bool OnWindowResize();

	void RenderGraphGrid(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
		ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, unsigned int indexCount);

	void RenderGraphPoints(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
		ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, unsigned int indexCount);

	void RenderTriangleMesh(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
		ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, unsigned int indexCount);

	ID3D11Device* GetPtrDevice() const;
	ID3D11DeviceContext* GetPtrDeviceContext() const;

	DirectX::XMMATRIX GetProjectionMatrix() const;
	DirectX::XMMATRIX GetOrthographicMatrix() const;

	uint32_t GetWindowWidth() const;
	uint32_t GetWindowHeight() const;

private:
	void SetBuffersGrid(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer);
	void SetBuffersTriangles(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer);
	void SetBuffersPoints(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer);
	bool SetShaderParameters(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM,
		const DirectX::XMMATRIX& projM);
	void RenderShader(int indexCount);

private:
	const HWND cHWnd;
	uint32_t cWndWidth;
	uint32_t cWndHeight;
	
	bool cFullscreen;
	bool cVSync;

	IDXGISwapChain* cPSwapChain;
	ID3D11Device* cPDevice;
	ID3D11DeviceContext* cPDeviceContext;
	ID3D11RenderTargetView* cPRenderTargetView;
	ID3D11Texture2D* cPDepthStencilBuffer;
	ID3D11DepthStencilState* cPDepthStencilState;
	ID3D11DepthStencilView* cPDepthStencilView;
	ID3D11RasterizerState* cPRasterState;
	ID3D11BlendState* cPBlendState;
	ID3D11GeometryShader* cPGeometryShader;
#ifdef _DEBUG
	ID3D11Debug *cPDebug;
#endif

	ColorShader* cPColorShader;

	DirectX::XMMATRIX  cOrthographicMatrix;
	DirectX::XMMATRIX  cProjectionMatrix;

};