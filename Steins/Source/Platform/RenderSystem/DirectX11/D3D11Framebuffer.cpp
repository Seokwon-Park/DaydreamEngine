#include "SteinsPCH.h"
#include "D3D11Framebuffer.h"
#include "D3D11Swapchain.h"

namespace Steins
{
	D3D11Framebuffer::D3D11Framebuffer(GraphicsDevice* _device, const FramebufferSpecification& _spec)
	{
		device = Cast<D3D11GraphicsDevice>(_device);
		if (_spec.swapChainTarget == true)
		{
			IDXGISwapChain* sc = Cast<D3D11SwapChain>(device->GetSwapChain(0))->GetDXGISwapChain();
			ComPtr<ID3D11Texture2D> backBuffer;
			sc->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
			STEINS_CORE_ASSERT(backBuffer, "Backbuffer is nullptr!");

			D3D11_TEXTURE2D_DESC textureDesc;
			backBuffer->GetDesc(&textureDesc);

			// 텍스처의 너비와 높이 변경
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			ID3D11RenderTargetView* rtv;
			device->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtv);
			renderTargetViews.push_back(rtv);
		}
		for (const FramebufferAttachmentSpecification& colorAttachment : _spec.colorAttachments)
		{

		}
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
		for (auto rtv : renderTargetViews)
		{
			rtv->Release();
		}
	}
	void D3D11Framebuffer::Bind()
	{
		device->GetContext()->OMSetRenderTargets(renderTargetViews.size(), renderTargetViews.data(), depthStencilView.Get());
	}
}