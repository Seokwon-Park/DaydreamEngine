#pragma once

#include "D3D11RenderDevice.h"
#include "Steins/Graphics/Core/RenderPass.h"

namespace Steins
{
	class D3D11Framebuffer;

	class D3D11RenderPass : public RenderPass
	{
	public:
		D3D11RenderPass(D3D11RenderDevice* _device, const RenderPassDesc& _desc);
		virtual ~D3D11RenderPass();

		virtual void Begin(Shared<Framebuffer> _framebuffer) override;
		virtual void End() override;

		virtual Shared<Framebuffer> CreateFramebuffer(const FramebufferDesc& _desc) override;
	private:
		D3D11RenderDevice* device;
		D3D11Framebuffer* currentFramebuffer;
	};
}