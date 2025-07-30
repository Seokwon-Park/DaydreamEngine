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
		void CreateAttachments();
		D3D11Framebuffer(D3D11RenderDevice* _device, RenderPass* _renderPass, D3D11Swapchain* _swapChain);
		virtual ~D3D11Framebuffer() override;

		virtual Shared<Texture2D> GetColorAttachmentTexture(UInt32 _index) override;
		virtual bool HasDepthAttachment() override { return depthAttachment != nullptr; }
		virtual void Resize(UInt32 _width, UInt32 _height) override;
		//virtual Shared<Texture2D> GetDepthAttachemntTexture(UInt32 _index) override;

		Array<ID3D11RenderTargetView*> GetRenderTargetViews() { return rtvs; }
		ComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return depthAttachment->GetDSV(); }

	private:
		D3D11RenderDevice* device;
		Array<Shared<D3D11Texture2D>> colorAttachments;
		Shared<D3D11Texture2D> depthAttachment;
		Array<ID3D11RenderTargetView*> rtvs;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
	};
}