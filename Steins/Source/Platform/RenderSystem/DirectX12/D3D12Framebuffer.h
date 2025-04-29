#pragma once

#include "Steins/Render/Framebuffer.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12SwapChain;

	class D3D12Framebuffer : public Framebuffer
	{
	public:
		D3D12Framebuffer(D3D12GraphicsDevice* _device, const FramebufferSpecification& _spec);
		D3D12Framebuffer(D3D12GraphicsDevice* _device, D3D12SwapChain* _swapChain);
		virtual ~D3D12Framebuffer() override {};

		virtual void Bind() const override;
		virtual void Clear(Color _color) {};

	private:
		D3D12GraphicsDevice* device;
		ComPtr<ID3D12CommandAllocator> commandAllocators[2];
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles; 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle; 
		std::vector<ComPtr<ID3D12Resource>> renderTargets;
		ComPtr<ID3D12Resource> depthAttachment;
	};
}

