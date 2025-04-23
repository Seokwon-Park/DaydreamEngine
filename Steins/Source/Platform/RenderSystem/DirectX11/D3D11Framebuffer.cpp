#include "SteinsPCH.h"
#include "D3D11Framebuffer.h"
#include "D3D11Swapchain.h"

namespace Steins
{
	D3D11Framebuffer::D3D11Framebuffer(D3D11GraphicsDevice* _device, const FramebufferSpecification& _spec)
	{
		device = _device;
		for (const FramebufferAttachmentSpecification& colorAttachment : _spec.colorAttachments)
		{

		}
	}
	D3D11Framebuffer::D3D11Framebuffer(D3D11GraphicsDevice* _device, D3D11SwapChain* _swapChain)
	{
		device = _device;
		IDXGISwapChain* dxgiSwapChain = _swapChain->GetDXGISwapChain();
		ComPtr<ID3D11Texture2D> backBuffer;
		dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
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
	void D3D11Framebuffer::Bind() const
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