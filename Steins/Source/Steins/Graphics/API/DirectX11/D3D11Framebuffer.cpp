#include "SteinsPCH.h"
#include "D3D11Framebuffer.h"
#include "D3D11Swapchain.h"
#include "D3D11Texture.h"

namespace Steins
{
	D3D11Framebuffer::D3D11Framebuffer(D3D11RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;
		
		width = _desc.width;
		height = _desc.height;

		const RenderPassDesc& renderPassDesc = _renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<D3D11Texture2D> colorTexture = MakeShared<D3D11Texture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			rtvs.push_back(colorTexture->GetRTV().Get());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil;

			Shared<D3D11Texture2D> depthTexture = MakeShared<D3D11Texture2D>(device, textureDesc);
			depthAttachment = depthTexture;
			depthStencilView = depthTexture->GetDSV();
		}
	}
	D3D11Framebuffer::D3D11Framebuffer(D3D11RenderDevice* _device, RenderPass* _renderPass, D3D11Swapchain* _swapChain)
	{
		device = _device;
		IDXGISwapChain* dxgiSwapchain = _swapChain->GetDXGISwapchain();
		ComPtr<ID3D11Texture2D> backBuffer;
		dxgiSwapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));

		Shared<D3D11Texture2D> backBufferTexture = MakeShared<D3D11Texture2D>(device, backBuffer);
		colorAttachments.push_back(backBufferTexture);
		STEINS_CORE_ASSERT(backBuffer, "Backbuffer is nullptr!");

		for (auto texture : colorAttachments)
		{
			rtvs.push_back(texture->GetRTV().Get());
		}
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
	}
	//void D3D11Framebuffer::Begin() const
	//{
	//	Array<ID3D11RenderTargetView*> rtv;
	//	for (auto texture : colorAttachments)
	//	{
	//		rtv.push_back(texture->GetRTV().Get());
	//	}
	//	device->GetContext()->OMSetRenderTargets(Cast<UINT>(rtv.size()), rtv.data(), nullptr);
	//}

	void* D3D11Framebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index]->GetSRV().Get();
	}
}