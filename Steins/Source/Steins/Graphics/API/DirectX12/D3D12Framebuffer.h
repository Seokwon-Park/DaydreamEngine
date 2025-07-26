#pragma once

#include "Steins/Graphics/Resources/Framebuffer.h"
#include "D3D12RenderDevice.h"
#include "D3D12Texture.h"

namespace Steins
{
	class D3D12Swapchain;

	class D3D12Framebuffer : public Framebuffer
	{
	public:
		D3D12Framebuffer(D3D12RenderDevice* _device, const FramebufferDesc& _spec);
		D3D12Framebuffer(D3D12RenderDevice* _device, D3D12Swapchain* _swapChain);
		virtual ~D3D12Framebuffer() override;

		virtual void Begin() const override;
		virtual void End() const override {};
		virtual void Clear(Color _color) override;

		ComPtr<ID3D12Resource> GetRenderTarget() { return colorAttachments[0]->GetTexture(); }

	private:
		D3D12RenderDevice* device;
		D3D12Swapchain* swapChain;
		//Array<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles; 
		//D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle; 
		Array<Shared<D3D12Texture2D>> colorAttachments;
		ComPtr<ID3D12Resource> depthAttachment;
	};
}

