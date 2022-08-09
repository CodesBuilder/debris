#include "../dx11.h"

#include <string>

static D3D_DRIVER_TYPE drvTypes[] = {
	D3D_DRIVER_TYPE_HARDWARE,
	D3D_DRIVER_TYPE_WARP,
	D3D_DRIVER_TYPE_REFERENCE,
	D3D_DRIVER_TYPE_SOFTWARE
};

static D3D_FEATURE_LEVEL featureLevels[] = {
	D3D_FEATURE_LEVEL_11_0
};

HRESULT DebrisRenderer::D3D11RenderContext::_initD2D() {
	HRESULT result;

	_dxgiDevice->SetMaximumFrameLatency(1);

	if (FAILED(result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory1), (void**)&_d2dFactory)))
		return result;

	if (FAILED(result = _d2dFactory->CreateDevice(_dxgiDevice.Get(), _d2dDevice.GetAddressOf())))
		return result;

	if (FAILED(result = _d2dDevice->CreateDeviceContext(
				   D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				   _d2dDeviceContext.GetAddressOf())))
		return result;

	if (FAILED(result = _dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&_dxgiBackBuffer))))
		return result;

	if (FAILED(result = _d2dDeviceContext->CreateBitmapFromDxgiSurface(
				   _dxgiBackBuffer.Get(),
				   D2D1::BitmapProperties1(
					   D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					   D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
				   _d2dTargetBitmap.GetAddressOf())))
		return result;

	_d2dDeviceContext->SetTarget(_d2dTargetBitmap.Get());
	_d2dDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);
	_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	if (FAILED((result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &_dwriteFactory))))
		return result;

	return ERROR_SUCCESS;
}

HRESULT DebrisRenderer::D3D11RenderContext::_initD3D() {
	HRESULT result;

	UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	flags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	{
		for (UINT i = 0; i < ARRAYSIZE(drvTypes); i++) {
			if (SUCCEEDED(result = D3D11CreateDevice(
							  nullptr, // DXGI Adapter
							  drvTypes[i],
							  nullptr,
							  flags,
							  featureLevels,
							  ARRAYSIZE(featureLevels),
							  D3D11_SDK_VERSION,
							  &_d3dDevice,
							  &_featureLevel,
							  &_d3dImmediateContext)))
				break;
		}
		if (FAILED(result))
			return result;
	}

	// Get DXGI resources
	{
		// Get DXGI Device
		if (FAILED(result = _d3dDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&_dxgiDevice)))
			return result;

		// Get DXGI Adapter
		if (FAILED(result = _dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf())))
			return result;

		// Get DXGI Factory
		if (FAILED(result = _dxgiAdapter->GetParent(
					   __uuidof(IDXGIFactory1),
					   (void**)_dxgiFactory.GetAddressOf())))
			return result;
	}

	// Initialize swap chain descriptor
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
		{
			swapChainDesc.Width = _window->getSize().x;
			swapChainDesc.Height = _window->getSize().y;
		}
		{
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}
		{
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
		}

		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

		swapChainDesc.Stereo = false;

		swapChainDesc.Flags = 0;

		if (FAILED(result = _dxgiFactory->CreateSwapChainForHwnd(
					   _d3dDevice.Get(),
					   _window->_getHwnd(),
					   &swapChainDesc,
					   nullptr,
					   nullptr,
					   _dxgiSwapChain.GetAddressOf())))
			return result;
	}

	// Disable ALT + Enter association
	_dxgiFactory->MakeWindowAssociation(
		_window->_getHwnd(),
		DXGI_MWA_NO_ALT_ENTER |
			DXGI_MWA_NO_WINDOW_CHANGES |
			DXGI_MWA_NO_PRINT_SCREEN);

	if (FAILED(result = _dxgiSwapChain->GetBuffer(
				   0,
				   __uuidof(ID3D11Texture2D),
				   (void**)_d3dBackBuffer.GetAddressOf())))
		return result;

	if (FAILED(result = _d3dDevice->CreateRenderTargetView(
				   _d3dBackBuffer.Get(),
				   nullptr,
				   _d3dRenderTargetView.GetAddressOf())))
		return result;

	return ERROR_SUCCESS;
}

HRESULT DebrisRenderer::D3D11RenderContext::_onResize() {
	HRESULT result;

	_d3dRenderTargetView.Reset();
	_d3dDepthStencilView.Reset();
	_d3dDepthStencilBuffer.Reset();

	ComPtr<ID3D11Texture2D> backBuffer;

	if (FAILED(result = _dxgiSwapChain->ResizeBuffers(
				   1,
				   _window->getSize().x,
				   _window->getSize().y,
				   DXGI_FORMAT_R8G8B8A8_UNORM,
				   0)))
		return result;

	if (FAILED(result = _dxgiSwapChain->GetBuffer(
				   0,
				   __uuidof(ID3D11Texture2D),
				   (void**)backBuffer.GetAddressOf())))
		return result;
	if (FAILED(result = _d3dDevice->CreateRenderTargetView(
				   backBuffer.Get(),
				   nullptr,
				   _d3dRenderTargetView.GetAddressOf())))
		return result;

	backBuffer.Reset();

	return _resize();
}

HRESULT DebrisRenderer::D3D11RenderContext::_resize() {
	HRESULT result;

	// Initialize depth stencil descriptor.
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _window->getSize().x;
	depthStencilDesc.Height = _window->getSize().y;

	depthStencilDesc.MipLevels = 1;

	depthStencilDesc.ArraySize = 1;

	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Create Depth Stencil View.
	if (FAILED(result = _d3dDevice->CreateTexture2D(
				   &depthStencilDesc,
				   NULL,
				   _d3dDepthStencilBuffer.GetAddressOf())))
		return result;

	if (FAILED(result = _d3dDevice->CreateDepthStencilView(
				   _d3dDepthStencilBuffer.Get(),
				   NULL,
				   _d3dDepthStencilView.GetAddressOf())))
		return result;

	_d3dImmediateContext->OMSetRenderTargets(
		1,
		_d3dRenderTargetView.GetAddressOf(),
		_d3dDepthStencilView.Get());

	_d3dViewport.TopLeftX = 0;
	_d3dViewport.TopLeftY = 0;
	_d3dViewport.Width = (float)_window->getSize().x;
	_d3dViewport.Height = (float)_window->getSize().y;
	_d3dViewport.MinDepth = 0.0f;
	_d3dViewport.MaxDepth = 1.0f;

	_d3dImmediateContext->RSSetViewports(1, &_d3dViewport);

	return ERROR_SUCCESS;
}

DebrisRenderer::D3D11RenderContext::D3D11RenderContext(Debris::Window* window) {
	HRESULT result;

	_window = (Debris::Native::BasicWindow*)window;

	if (FAILED(result = _initD3D())) {
		throw std::runtime_error("Failed to initialize Direct3D with HRESULT error code: " + std::to_string(result));
	}

	if (FAILED(result = _initD2D())) {
		throw std::runtime_error("Failed to initialize Direct2D with HRESULT error code: " + std::to_string(result));
	}

	if (FAILED(result = _resize())) {
		throw std::runtime_error("Failed to resize with HRESULT error code: " + std::to_string(result));
	}
}

DebrisRenderer::D3D11RenderContext::~D3D11RenderContext() {
	_d3dImmediateContext->ClearState();
}

Debris::Window* DebrisRenderer::D3D11RenderContext::getWindow() {
	return _window;
}

Debris::IRenderContext* DebrisRenderer::D3D11Renderer::createContext(Window* window) {
	if (typeid(window) != typeid(Window*))
		throw std::invalid_argument("Required Window element");
	D3D11RenderContext* context = new D3D11RenderContext(window);
	return (Debris::IRenderContext*)context;
}

void DebrisRenderer::D3D11RenderContext::beginDraw() {
	lock();

	_d2dDeviceContext->BeginDraw();
	_d3dImmediateContext->ClearDepthStencilView(
		_d3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void DebrisRenderer::D3D11RenderContext::endDraw() {
	unlock();
	_d2dDeviceContext->EndDraw();
	_dxgiSwapChain->Present(0, 0);
}

void DebrisRenderer::D3D11RenderContext::drawRect(Pos2D& pos, Size2D& size, ColorF& color) {
	ComPtr<ID2D1SolidColorBrush> brush;
	if (FAILED(_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a),&brush)))
		throw std::runtime_error("Error creating brush");
	_d2dDeviceContext->FillRectangle(D2D1::RectF(pos.x, pos.y, pos.x + size.x, pos.y + size.y), brush.Get());
}

void DebrisRenderer::D3D11RenderContext::clear(ColorI& color) {
}

void DebrisRenderer::D3D11RenderContext::clear(ColorF& color) {
	_d2dDeviceContext->Clear(D2D1_COLOR_F(D2D1::ColorF(color.r, color.g, color.b, color.a)));
}

void DebrisRenderer::D3D11RenderContext::onResize() {
	_resize();
}

DebrisRenderer::D3D11Renderer::D3D11Renderer() {
}

DebrisRenderer::D3D11Renderer::~D3D11Renderer() {
}
