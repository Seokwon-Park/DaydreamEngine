#pragma once

#include "Steins/Render/Framebuffer.h"
#include "Steins/Render/Texture.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(GraphicsDevice* _device, const FramebufferSpecification& _spec);

		virtual void Bind() override;

		std::vector<ComPtr<ID3D11RenderTargetView>>& GetRTVs() { return renderTargetViews; }
	private:
		D3D11GraphicsDevice* device;
		std::vector<ComPtr<ID3D11RenderTargetView>> renderTargetViews; 
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		std::vector<ComPtr<ID3D11Texture2D>> colorAttachments; 
		ComPtr<ID3D11Texture2D> depthAttachment;
	};
}