#include "DirectX11.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment( lib, "dxguid.lib") 

#include <D3DCommon.h>
#include <DXGIDebug.h>

DirectX11::DirectX11(HWND hWnd, bool cull_back)
	:
	cHWnd(hWnd),
	cPSwapChain(nullptr),
	cPDevice(nullptr),
	cPDeviceContext(nullptr),
	cPRenderTargetView(nullptr),
	cPDepthStencilBuffer(nullptr),
	cPDepthStencilState(nullptr),
	cPDepthStencilView(nullptr),
	cPRasterState(nullptr),
	cPBlendState(nullptr),
	cPColorShader(nullptr),
	cPGeometryShader(nullptr),
	cVSync(true),
	cFullscreen(false)
#ifdef _DEBUG
	, cPDebug(nullptr)
#endif
{
	RECT r = {};
	GetClientRect(hWnd, &r);

	cWndWidth = r.right - r.left;
	cWndHeight = r.bottom - r.top;

	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	//int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_RASTERIZER_DESC rasterDesc;


	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		assert(0);
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		assert(0);
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		assert(0);
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		assert(1);
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		assert(0);
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		assert(0);
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)cWndWidth)
		{
			if (displayModeList[i].Height == (unsigned int)cWndHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	numerator = 60;
	denominator = 1;

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		assert(1);
	}

	// Store the dedicated video card memory in megabytes.
	//int videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	//	char videoCardDescription[250];
	// Convert the name of the video card to a character array and store it.
	//error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	/*if (error != 0)
	{
	assert(1);
	}*/

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;


	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = cWndWidth;
	swapChainDesc.BufferDesc.Height = cWndHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM;

	bool vsync_enabled = true;
	// Set the refresh rate of the back buffer.
	if (vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = cHWnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 4;


	// Set to full screen or windowed mode.
	if (cFullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	DWORD flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, &featureLevel, 1, D3D11_SDK_VERSION,
		&cPDevice, NULL, &cPDeviceContext);
	if (FAILED(result))
	{
		assert(0);
	}



#ifdef _DEBUG
	cPDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&cPDebug));
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

#ifdef _DEBUGs
	ID3D11InfoQueue* infoQueue = nullptr;
	cPDevice->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));
	if (infoQueue)
	{
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_WARNING, TRUE);
		infoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
		infoQueue->Release();
		infoQueue = nullptr;
	}

#endif

	IDXGIDevice* dxgiDevice = 0;
	result = cPDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(result))
	{
		assert(0);
	}

	IDXGIAdapter* dxgiAdapter = 0;
	result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(result))
	{
		assert(0);
	}

	IDXGIFactory* dxgiFactory = 0;
	result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(result))
	{
		assert(0);
	}

#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

	swapChainDesc.SampleDesc.Quality = 0;

	result = dxgiFactory->CreateSwapChain(cPDevice, &swapChainDesc, &cPSwapChain);
	if (FAILED(result))
	{
		assert(0);
	}
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	dxgiDevice->Release();
	dxgiDevice = nullptr;

	dxgiAdapter->Release();
	dxgiAdapter = nullptr;

	dxgiFactory->Release();
	dxgiFactory = nullptr;
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	

	OnWindowResize();

	D3D11_BLEND_DESC blendStateDescription;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	// Create an alpha enabled blend state description.

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDescription.AlphaToCoverageEnable = false;

	// Create the blend state using the description.
	result = cPDevice->CreateBlendState(&blendStateDescription, &cPBlendState);
	if (FAILED(result))
	{
		assert(1);
	}

	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	cPDeviceContext->OMSetBlendState(cPBlendState, blendFactor, 0xffffffff);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = (cull_back) ? D3D11_CULL_BACK : D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = cPDevice->CreateRasterizerState(&rasterDesc, &cPRasterState);
	if (FAILED(result))
	{
		assert(0);
	}

	// Now set the rasterizer state.
	cPDeviceContext->RSSetState(cPRasterState);

	ID3D10Blob* geomShaderBuffer = nullptr;
	ID3D10Blob* errorMessage = nullptr;

	result = D3DCompileFromFile(L"Color.gs", 0, 0, "ColorGS", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS,
		0, &geomShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			Shader::OutputShaderErrorMessage(errorMessage, NULL, L"Color.gs");
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(NULL, L"Color.gs", L"Missing Shader File", MB_OK);
		}

		MessageBox(NULL, L"Erro", L"Erro", 0);
	}
	
	result = cPDevice->CreateGeometryShader(geomShaderBuffer->GetBufferPointer(), geomShaderBuffer->GetBufferSize(), NULL, &cPGeometryShader);
	
	if(errorMessage)
		errorMessage->Release();
	errorMessage = nullptr;
	geomShaderBuffer->Release();
	geomShaderBuffer = nullptr;
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
}

DirectX11::~DirectX11()
{
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

	cPDeviceContext->ClearState();
	
	if (cPColorShader)
	{
		delete cPColorShader;
		cPColorShader = nullptr;
	}

	if (cPBlendState)
	{
		cPBlendState->Release();
		cPBlendState = nullptr;
	}

	if (cPGeometryShader)
	{
		cPGeometryShader->Release();
		cPGeometryShader = nullptr;
	}

	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (cPSwapChain)
	{
		cPSwapChain->SetFullscreenState(false, NULL);
	}

	if (cPDepthStencilBuffer)
	{
		cPDepthStencilBuffer->Release();
		cPDepthStencilBuffer = nullptr;
	}

	if (cPDepthStencilView)
	{
		cPDepthStencilView->Release();
		cPDepthStencilView = nullptr;
	}
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	if (cPDepthStencilState)
	{
		cPDepthStencilState->Release();
		cPDepthStencilState = nullptr;
	}
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	if (cPRenderTargetView)
	{
		cPRenderTargetView->Release();
		cPRenderTargetView = nullptr;
	}

	if (cPRasterState)
	{
		cPRasterState->Release();
		cPRasterState = nullptr;
	}

	if (cPSwapChain)
	{
		cPSwapChain->Release();
		cPSwapChain = nullptr;
	}

	if (cPDeviceContext)
	{
		cPDeviceContext->Release();
		cPDeviceContext = nullptr;
	}

#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
#ifdef _DEBUG
	if (cPDebug)
	{
		cPDebug->Release();
		cPDebug = nullptr;
	}
#endif

	if (cPDevice)
	{
		cPDevice->Release();
		cPDevice = nullptr;
	}
}

void DirectX11::OpenScene()
{
	float color[4];

	// Setup the color to clear the buffer to.
	//color[0] = 135.0f / 256.0f;
	//color[1] = 206.0f / 256.0f;
	//color[2] = 235.0f / 256.0f;
	//color[3] = 0.0f; // XX

	color[0] = 0.8f;
	color[1] = 0.8f;
	color[2] = 0.8f;
	color[3] = 0.0f; // XX

					 // Clear the back buffer.
	cPDeviceContext->ClearRenderTargetView(cPRenderTargetView, color);

	cPDeviceContext->ClearDepthStencilView(cPDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX11::CloseScene()
{
	if (cVSync)
	{
		// Lock to screen refresh rate.
		cPSwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		cPSwapChain->Present(0, 0);
	}
}

bool DirectX11::OnWindowResize()
{
	RECT r = {};
	GetClientRect(cHWnd, &r);

	cWndWidth = r.right - r.left;
	cWndHeight = r.bottom - r.top;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_VIEWPORT viewport;

	ID3D11Texture2D* backBufferPtr;

	HRESULT result;
	cPDeviceContext->OMSetRenderTargets(0, NULL, NULL);

	cPDeviceContext->Flush();

#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

	if (cPRenderTargetView)
		cPRenderTargetView->Release();
	if (cPDepthStencilView)
		cPDepthStencilView->Release();
	if (cPDepthStencilBuffer)
		cPDepthStencilBuffer->Release();
	if (cPDepthStencilState)
	{
		cPDepthStencilState->Release();
		cPDepthStencilState = nullptr;
	}

#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif


	cPSwapChain->ResizeBuffers(0, cWndWidth, cWndHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// Get the pointer to the back buffer.
	result = cPSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		assert(0);
	}
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Create the render target view with the back buffer pointer.
	result = cPDevice->CreateRenderTargetView(backBufferPtr, NULL, &cPRenderTargetView);
	if (FAILED(result))
	{
		assert(0);
	}
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = cWndWidth;
	depthBufferDesc.Height = cWndHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 4;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = cPDevice->CreateTexture2D(&depthBufferDesc, NULL, &cPDepthStencilBuffer);
	if (FAILED(result))
	{
		assert(0);
	}
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	//result = cPDevice->CreateDepthStencilState(&depthStencilDesc, &cPDepthDisabledStencilState);
	if (FAILED(result))
	{
		assert(0);
	}

/*#ifdef _DEBUG
	const char c_szName[] = "disabled";
	cPDepthDisabledStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName) - 1, c_szName);
#endif*/

	depthStencilDesc.DepthEnable = true;

	result = cPDevice->CreateDepthStencilState(&depthStencilDesc, &cPDepthStencilState);
	if (FAILED(result))
	{
		assert(0);
	}

#ifdef _DEBUG
	const char c_szName2[] = "enabled";
	cPDepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(c_szName2) - 1, c_szName2);
#endif
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Set the depth stencil state.
	cPDeviceContext->OMSetDepthStencilState(cPDepthStencilState, 1);
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;// D3D11_DSV_DIMENSION_TEXTURE2D D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
#ifdef _DEBUG
	//cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Create the depth stencil view.
	result = cPDevice->CreateDepthStencilView(cPDepthStencilBuffer, &depthStencilViewDesc, &cPDepthStencilView);
	if (FAILED(result))
	{
		assert(0);
	}
#ifdef _DEBUG
	cPDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	cPDeviceContext->OMSetRenderTargets(1, &cPRenderTargetView, cPDepthStencilView);

	// Setup the viewport for rendering.
	viewport.Width = (float)cWndWidth;
	viewport.Height = (float)cWndHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	cPDeviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = (float)cWndWidth / (float)cWndHeight;
	
	if (cPColorShader == nullptr)
	{
		cPColorShader = new ColorShader(cHWnd, *this);
	}

	// Create an orthographic projection matrix for 2D rendering.
	cOrthographicMatrix = DirectX::XMMatrixOrthographicLH((float)cWndWidth, (float)cWndHeight, 0.0f, 1.0f);

	// Create the projection matrix for 3D rendering.
	cProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.001f, FLT_MAX);

	return true;
}

ID3D11Device* DirectX11::GetPtrDevice() const
{
	return cPDevice;
}
ID3D11DeviceContext* DirectX11::GetPtrDeviceContext() const
{
	return cPDeviceContext;
}

DirectX::XMMATRIX DirectX11::GetProjectionMatrix() const
{
	return cProjectionMatrix;
}
DirectX::XMMATRIX DirectX11::GetOrthographicMatrix() const
{
	return cOrthographicMatrix;
}

uint32_t DirectX11::GetWindowWidth() const
{
	return cWndWidth;
}
uint32_t DirectX11::GetWindowHeight() const
{
	return cWndHeight;
}

void DirectX11::RenderGraphGrid(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
	ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, unsigned int indexCount)
{
	cPDeviceContext->OMSetDepthStencilState(cPDepthStencilState, 1);

	cPDeviceContext->IASetInputLayout(cPColorShader->GetPLayout());

	cPDeviceContext->VSSetShader(cPColorShader->GetPVertexShader(), NULL, 0);
	cPDeviceContext->PSSetShader(cPColorShader->GetPPixelShader(), NULL, 0);

	cPDeviceContext->GSSetShader(nullptr, NULL, 0);

	SetBuffersGrid(pVertexBuffer, pIndexBuffer);

	SetShaderParameters(viewM, worldM, projM);

	RenderShader(indexCount);
}

void DirectX11::RenderTriangleMesh(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
	ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, unsigned int indexCount)
{
	cPDeviceContext->OMSetDepthStencilState(cPDepthStencilState, 1);

	cPDeviceContext->IASetInputLayout(cPColorShader->GetPLayout());

	cPDeviceContext->VSSetShader(cPColorShader->GetPVertexShader(), NULL, 0);
	cPDeviceContext->PSSetShader(cPColorShader->GetPPixelShader(), NULL, 0);

	cPDeviceContext->GSSetShader(nullptr, NULL, 0);

	SetBuffersTriangles(pVertexBuffer, pIndexBuffer);

	SetShaderParameters(viewM, worldM, projM);

	RenderShader(indexCount);
}

void DirectX11::RenderGraphPoints(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
	ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, unsigned int indexCount)
{
	cPDeviceContext->OMSetDepthStencilState(cPDepthStencilState, 1);

	cPDeviceContext->IASetInputLayout(cPColorShader->GetPLayout());

	cPDeviceContext->VSSetShader(cPColorShader->GetPVertexShader(), NULL, 0);
	cPDeviceContext->PSSetShader(cPColorShader->GetPPixelShader(), NULL, 0);
	cPDeviceContext->GSSetShader(cPGeometryShader, NULL, 0);

	SetBuffersPoints(pVertexBuffer, pIndexBuffer);

	SetShaderParameters(viewM, worldM, projM);

	RenderShader(indexCount);
}

void DirectX11::SetBuffersGrid(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	cPDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	cPDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	cPDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
}

void DirectX11::SetBuffersTriangles(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	cPDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	cPDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	cPDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DirectX11::SetBuffersPoints(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	cPDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	cPDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	cPDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
}
bool DirectX11::SetShaderParameters(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM,
	const DirectX::XMMATRIX& projM)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	DirectX::XMMATRIX orthoMatrix, worldMatrix, viewMatrix;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldM);
	viewMatrix = XMMatrixTranspose(viewM);
	orthoMatrix = XMMatrixTranspose(projM);

	auto pMatrixBuffer = cPColorShader->GetPMatrixBuffer();

	// Lock the constant buffer so it can be written to.
	result = cPDeviceContext->Map(pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->worldMatrix = worldMatrix;
	dataPtr->viewMatrix = viewMatrix;
	dataPtr->orthoMatrix = orthoMatrix;

	// Unlock the constant buffer.
	cPDeviceContext->Unmap(pMatrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	cPDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &pMatrixBuffer);

	// Set shader texture resource in the pixel shader.
//	cPDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	return true;

}
void DirectX11::RenderShader(int indexCount)
{
	//cPDeviceContext->Draw(indexCount, 0);
	cPDeviceContext->DrawIndexed(indexCount, 0, 0);
}