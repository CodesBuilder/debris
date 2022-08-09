#pragma once

#ifdef _WIN32

#include <debris/native/win32/window.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d2d1_1.h>
#include <d3d11.h>
#include <dwrite_1.h>
#include <dxgi1_2.h>
#include <wrl.h>

namespace DebrisRenderer {
	using namespace Debris;
	using Microsoft::WRL::ComPtr;

	class D3D11RenderContext : public BasicRenderContext {
	protected:
		ComPtr<ID3D11Device> _d3dDevice;
		ComPtr<ID3D11DeviceContext> _d3dImmediateContext;
		ComPtr<IDXGISwapChain1> _dxgiSwapChain;
		ComPtr<ID3D11Texture2D> _d3dDepthStencilBuffer;
		ComPtr<ID3D11RenderTargetView> _d3dRenderTargetView;
		ComPtr<ID3D11DepthStencilView> _d3dDepthStencilView;
		ComPtr<ID3D11Texture2D> _d3dBackBuffer;
		D3D11_VIEWPORT _d3dViewport;

		ComPtr<IDXGIDevice1> _dxgiDevice;
		ComPtr<IDXGIAdapter> _dxgiAdapter;
		ComPtr<IDXGIFactory2> _dxgiFactory;
		ComPtr<IDXGISurface> _dxgiBackBuffer;

		ComPtr<ID2D1Factory1> _d2dFactory;
		ComPtr<ID2D1Device> _d2dDevice;
		ComPtr<ID2D1DeviceContext> _d2dDeviceContext;
		ComPtr<ID2D1Bitmap1> _d2dTargetBitmap;

		ComPtr<IDWriteFactory> _dwriteFactory;

		Debris::Native::BasicWindow* _window;

		D3D_FEATURE_LEVEL _featureLevel;

		std::mutex _lock;

		HRESULT _initD2D();
		HRESULT _initD3D();

		HRESULT _onResize();
		HRESULT _resize();

		friend class D3D11Renderer;

	public:
		D3D11RenderContext(Debris::Window* hWnd);
		virtual ~D3D11RenderContext();

		virtual void beginDraw() override;
		virtual void endDraw() override;

		virtual void drawRect( Pos2D& pos, Size2D& size, ColorF& color) override;
		virtual inline void drawRect(Pos2D& pos, Size2D& size, ColorI& color) override {
			drawRect(pos, size, (ColorF)color);
		}

		virtual void clear( ColorI& color) override;
		virtual void clear( ColorF& color) override;

		virtual void onResize() override;

		virtual Debris::Window* getWindow();
	};

	class D3D11Renderer : public IRenderer {
	public:
		D3D11Renderer();
		virtual ~D3D11Renderer() override;

		virtual IRenderContext* createContext(Window* window) override;

		D3D11Renderer& operator=(const D3D11Renderer&) = delete;
	};
}

#endif
