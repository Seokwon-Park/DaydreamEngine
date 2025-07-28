#pragma once

#include "Steins/Graphics/Resources/Framebuffer.h"
#include "Steins/Graphics/Resources/Texture.h"
#include "D3D11RenderDevice.h"
#include "D3D11Texture.h"
#include "D3D11RenderPass.h"

namespace Steins
{
	class D3D11Swapchain;

	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(D3D11RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc);
		D3D11Framebuffer(D3D11RenderDevice* _device, RenderPass* _renderPass, D3D11Swapchain* _swapChain);
		virtual ~D3D11Framebuffer() override;

		virtual void* GetColorAttachmentTexture(UInt32 _index) override;

		Array<ID3D11RenderTargetView*> GetRenderTargetViews() { return rtvs; }

	private:
		D3D11RenderDevice* device;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		Array<Shared<D3D11Texture2D>> colorAttachments;
		Array<ID3D11RenderTargetView*> rtvs;
		Shared<D3D11Texture2D> depthAttachment;
	};
}