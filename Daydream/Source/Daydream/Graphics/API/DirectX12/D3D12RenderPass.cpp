#include "DaydreamPCH.h"
#include "D3D12RenderPass.h"
#include "D3D12Framebuffer.h"

#include "D3D12Utility.h"

namespace Daydream
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
		currentFramebuffer = static_cast<D3D12Framebuffer*>(_framebuffer.get());

		for (auto tex : currentFramebuffer->GetColorAttachmentsRef())
		{
			device->TransitionResourceState(device->GetCommandList(), tex->GetID3D12Resource(), tex->GetCurrentState(),
				D3D12_RESOURCE_STATE_RENDER_TARGET);
			tex->SetCurrentState(D3D12_RESOURCE_STATE_RENDER_TARGET);
		}

		if (currentFramebuffer->HasDepthAttachment())
		{
			auto dsv = currentFramebuffer->GetDepthAttachment();
			device->TransitionResourceState(device->GetCommandList(), dsv->GetID3D12Resource(), dsv->GetCurrentState(),
				D3D12_RESOURCE_STATE_DEPTH_WRITE);
			dsv->SetCurrentState(D3D12_RESOURCE_STATE_DEPTH_WRITE);
		}

		Array<D3D12_CPU_DESCRIPTOR_HANDLE>& rtHandles = currentFramebuffer->GetRenderTargetHandles();
		//
		//for (auto texture : currentFramebuffer->GetColorAttachments())
		//{
		//	device->TransitionResourceState(texture->GetID3D12Resource(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
		//}
		//if (currentFramebuffer->HasDepthAttachment())
		//{
		//	device->TransitionResourceState(currentFramebuffer->GetDepthAttachment()->GetID3D12Resource(), D3D12_RESOURCE_STATE_COMMON,
		//		D3D12_RESOURCE_STATE_DEPTH_WRITE);
		//}

		for (auto rtHandle : rtHandles)
		{
			device->GetCommandList()->ClearRenderTargetView(rtHandle, clearColor.color, 0, nullptr);
		}

		if (currentFramebuffer->HasDepthAttachment())
		{
			device->GetCommandList()->ClearDepthStencilView(currentFramebuffer->GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
			device->GetCommandList()->OMSetRenderTargets((UINT)rtHandles.size(), rtHandles.data(), false, &currentFramebuffer->GetDepthStencilView());
		}
		else
		{
			device->GetCommandList()->OMSetRenderTargets((UINT)rtHandles.size(), rtHandles.data(), false, nullptr);
		}

		D3D12_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = _framebuffer->GetWidth();
		rect.bottom = _framebuffer->GetHeight();

		device->GetCommandList()->RSSetScissorRects(1, &rect);

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
		//for (auto texture : currentFramebuffer->GetColorAttachments())
		//{
		//	device->TransitionResourceState(texture->GetID3D12Resource(), D3D12_RESOURCE_STATE_RENDER_TARGET, 
		//		D3D12_RESOURCE_STATE_COMMON);
		//}
		//if (currentFramebuffer->HasDepthAttachment())
		//{
		//	device->TransitionResourceState(currentFramebuffer->GetDepthAttachment()->GetID3D12Resource(), 
		//		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		//		D3D12_RESOURCE_STATE_COMMON);
		//}

	}
	Shared<Framebuffer> D3D12RenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
	{
		return MakeShared<D3D12Framebuffer>(device, this, _desc);
	}
}
