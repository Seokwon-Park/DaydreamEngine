#include "SteinsPCH.h"
#include "D3D12RenderPass.h"
#include "D3D12Framebuffer.h"

namespace Steins
{
	D3D12RenderPass::D3D12RenderPass(D3D12RenderDevice* _device, const RenderPassDesc& _desc)
	{
		device = _device;
		desc = _desc;
	}
	D3D12RenderPass::~D3D12RenderPass()
	{
		device = nullptr;
	}
	void D3D12RenderPass::Begin(Shared<Framebuffer> _framebuffer)
	{
		currentFramebuffer = static_pointer_cast<D3D12Framebuffer>(_framebuffer);
		Array<D3D12_CPU_DESCRIPTOR_HANDLE> rtHandles = currentFramebuffer->GetRenderTargetHandles();
		for (auto rtHandle : rtHandles)
		{
			device->GetCommandList()->ClearRenderTargetView(rtHandle, clearColor.color, 0, nullptr);
		}
		if (currentFramebuffer->HasDepthAttachment())
		{
			
			device->GetCommandList()->ClearDepthStencilView(currentFramebuffer->GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
			device->GetCommandList()->OMSetRenderTargets(rtHandles.size(), rtHandles.data(), false, &currentFramebuffer->GetDepthStencilView());
		}
		else
		{
			device->GetCommandList()->OMSetRenderTargets(rtHandles.size(), rtHandles.data(), false, nullptr);
		}

		D3D12_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(_framebuffer->GetWidth());
		viewport.Height = static_cast<float>(_framebuffer->GetHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		device->GetCommandList()->RSSetViewports(1, &viewport);
	}
	void D3D12RenderPass::End()
	{
	}
	Shared<Framebuffer> D3D12RenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
	{
		return MakeShared<D3D12Framebuffer>(device, this, _desc);
	}
}
