#pragma once

#include "Steins/Graphics/Resources/Framebuffer.h"
#include "Steins/Graphics/Resources/Texture.h"
#include "D3D11RenderDevice.h"

namespace Steins
{
	class D3D11SwapChain;

	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(D3D11RenderDevice* _device, const FramebufferDesc& _spec);
		D3D11Framebuffer(D3D11RenderDevice* _device, D3D11SwapChain* _swapChain);
		virtual ~D3D11Framebuffer() override;

		virtual void Begin() const override;
		virtual void End() const override {};
		virtual void Clear(Color _color) override;

	private:
		D3D11RenderDevice* device;
		Array<ID3D11RenderTargetView*> renderTargetViews;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		Array<ComPtr<ID3D11Texture2D>> colorAttachments;
		ComPtr<ID3D11Texture2D> depthAttachment;
	};
}