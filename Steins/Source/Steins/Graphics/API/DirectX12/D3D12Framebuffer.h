#pragma once

#include "Steins/Graphics/Resources/Framebuffer.h"
#include "D3D12RenderDevice.h"

namespace Steins
{
	class D3D12SwapChain;

	class D3D12Framebuffer : public Framebuffer
	{
	public:
		D3D12Framebuffer(D3D12RenderDevice* _device, const FramebufferDesc& _spec);
		D3D12Framebuffer(D3D12RenderDevice* _device, D3D12SwapChain* _swapChain);
		virtual ~D3D12Framebuffer() override {};

		virtual void Begin() const override;
		virtual void End() const override {};
		virtual void Clear(Color _color) override;

		Array<ComPtr<ID3D12Resource>>& GetRenderTargets() { return renderTargets; }

	private:
		D3D12RenderDevice* device;
		D3D12SwapChain* swapChain;
		Array<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles; 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle; 
		Array<ComPtr<ID3D12Resource>> renderTargets;
		ComPtr<ID3D12Resource> depthAttachment;
	};
}

