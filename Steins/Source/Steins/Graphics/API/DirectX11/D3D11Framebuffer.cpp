#include "SteinsPCH.h"
#include "D3D11Framebuffer.h"
#include "D3D11Swapchain.h"

namespace Steins
{
	D3D11Framebuffer::D3D11Framebuffer(D3D11RenderDevice* _device, const FramebufferDesc& _spec)
	{
		device = _device;

	}
	D3D11Framebuffer::D3D11Framebuffer(D3D11RenderDevice* _device, D3D11Swapchain* _swapChain)
	{
		device = _device;
		IDXGISwapChain* dxgiSwapchain = _swapChain->GetDXGISwapchain();
		ComPtr<ID3D11Texture2D> backBuffer;
		dxgiSwapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		STEINS_CORE_ASSERT(backBuffer, "Backbuffer is nullptr!");

		//D3D11_TEXTURE2D_DESC textureDesc;
		//backBuffer->GetDesc(&textureDesc);

		//// 텍스처의 너비와 높이 변경
		//textureDesc.SampleDesc.Count = 1;
		//textureDesc.SampleDesc.Quality = 0;
		//textureDesc.Usage = D3D11_USAGE_DEFAULT;
		ID3D11RenderTargetView* rtv;
		device->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtv);
		renderTargetViews.push_back(rtv);
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
		for (auto rtv : renderTargetViews)
		{
			rtv->Release();
		}
	}
	void D3D11Framebuffer::Begin() const
	{
		device->GetContext()->OMSetRenderTargets(Cast<UINT>(renderTargetViews.size()), renderTargetViews.data(), nullptr);
	}

	void D3D11Framebuffer::Clear(Color _color)
	{
		for (auto rtv : renderTargetViews)
		{
			device->GetContext()->ClearRenderTargetView(rtv, _color.color);
		}
	}
}