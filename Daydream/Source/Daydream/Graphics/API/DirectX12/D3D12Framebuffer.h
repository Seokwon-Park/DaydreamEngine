#pragma once

#include "Daydream/Graphics/Resources/Framebuffer.h"
#include "D3D12RenderDevice.h"
#include "D3D12RenderPass.h"
#include "D3D12Texture.h"

namespace Daydream
{
	class D3D12Swapchain;

	class D3D12Framebuffer : public Framebuffer
	{
	public:
		D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc);
		D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, D3D12Swapchain* _swapChain, ID3D12Resource* _swapchainImage);
		virtual ~D3D12Framebuffer() override;

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) override;
		virtual bool HasDepthAttachment() override { return depthAttachment != nullptr; };
		virtual void Resize(UInt32 _width, UInt32 _height) override;

		Array<Shared<D3D12Texture2D>>& GetColorAttachmentsRef() { return colorAttachments; }
		const Shared<D3D12Texture2D>& GetDepthAttachment() { return depthAttachment; }

		const D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthStencilView() { return depthAttachment->GetDSVCPUHandle(); }
		Array<D3D12_CPU_DESCRIPTOR_HANDLE>& GetRenderTargetHandles() { return renderTargetHandles; }
		void CreateAttachments();

	private:
		D3D12RenderDevice* device;
		D3D12_CPU_DESCRIPTOR_HANDLE swapchainRTVHandle;
		Array<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles; 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle; 
		Array<Shared<D3D12Texture2D>> colorAttachments;
		Shared<D3D12Texture2D> depthAttachment;

		Array<Shared<D3D12Texture2D>> oldAttachments;
		
	};
}

