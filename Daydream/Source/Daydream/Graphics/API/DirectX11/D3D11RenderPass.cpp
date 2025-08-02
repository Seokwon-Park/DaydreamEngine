#include "DaydreamPCH.h"
#include "D3D11RenderPass.h"
#include "D3D11Framebuffer.h"

namespace Daydream
{
	D3D11RenderPass::D3D11RenderPass(D3D11RenderDevice* _device, const RenderPassDesc& _desc)
	{
		device = _device;
		desc = _desc;
	}
	D3D11RenderPass::~D3D11RenderPass()
	{
		device = nullptr;
	}
	void D3D11RenderPass::Begin(Shared<Framebuffer> _framebuffer)
	{
		currentFramebuffer = static_cast<D3D11Framebuffer*>(_framebuffer.get());
		Array<ID3D11RenderTargetView*> rtvs = currentFramebuffer->GetRenderTargetViews();
		for (auto rtv : rtvs)
		{
			device->GetContext()->ClearRenderTargetView(rtv, clearColor.color);
		}
		if (currentFramebuffer->HasDepthAttachment())
		{
			device->GetContext()->ClearDepthStencilView(currentFramebuffer->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			device->GetContext()->OMSetRenderTargets((UInt32)rtvs.size(), rtvs.data(), currentFramebuffer->GetDepthStencilView().Get());
		}
		else
		{
			device->GetContext()->OMSetRenderTargets((UInt32)rtvs.size(), rtvs.data(), nullptr);
		}

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Cast<Float32>(_framebuffer->GetWidth());
		viewport.Height = Cast<Float32>(_framebuffer->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		device->GetContext()->RSSetViewports(1, &viewport);
	}
	void D3D11RenderPass::End()
	{
		device->GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
	}
	Shared<Framebuffer> D3D11RenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
	{
		return MakeShared<D3D11Framebuffer>(device, this, _desc);
	}
}
