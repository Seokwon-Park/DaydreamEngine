#pragma once

#include "Steins/Graphics/Resources/Framebuffer.h"
#include "Steins/Graphics/Resources/Texture.h"
#include "D3D11RenderDevice.h"
#include "D3D11Texture.h"

namespace Steins
{
	class D3D11Swapchain;

	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(D3D11RenderDevice* _device, const FramebufferDesc& _spec);
		D3D11Framebuffer(D3D11RenderDevice* _device, D3D11Swapchain* _swapChain);
		virtual ~D3D11Framebuffer() override;

		virtual void Begin() const override;
		virtual void End() const override {};
		virtual void Clear(Color _color) override;

	private:
		D3D11RenderDevice* device;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		Array<Shared<D3D11Texture2D>> colorAttachments;
		Shared<D3D11Texture2D> depthAttachment;
	};
}