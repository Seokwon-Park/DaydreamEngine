#pragma once

#include "D3D12RenderDevice.h"
#include "Daydream/Graphics/Core/RenderPass.h"

namespace Daydream
{
	class D3D12Framebuffer;

	class D3D12RenderPass : public RenderPass
	{
	public:
		D3D12RenderPass(D3D12RenderDevice* _device, const RenderPassDesc& _desc);
		virtual ~D3D12RenderPass();

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) override;
	private:
		D3D12RenderDevice* device;
		D3D12Framebuffer* currentFramebuffer;
	};
}  