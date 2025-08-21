#include "DaydreamPCH.h"
#include "D3D11Framebuffer.h"
#include "D3D11Swapchain.h"
#include "D3D11Texture.h"

namespace Daydream
{
	D3D11Framebuffer::D3D11Framebuffer(D3D11RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;
		renderPass = _renderPass;

		CreateAttachments();
	}
	void D3D11Framebuffer::CreateAttachments()
	{
		const RenderPassDesc& renderPassDesc = renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			if (colorAttachmentDesc.isSwapchain) continue;
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<D3D11Texture2D> colorTexture = MakeShared<D3D11Texture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			renderTargetViews.push_back(colorTexture->GetRTV().Get());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
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
		renderPass = _renderPass;

		IDXGISwapChain* dxgiSwapchain = _swapChain->GetDXGISwapchain();
		ComPtr<ID3D11Texture2D> backBuffer;
		dxgiSwapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		D3D11_TEXTURE2D_DESC backBufferDesc;
		backBuffer->GetDesc(&backBufferDesc);

		width = backBufferDesc.Width;
		height = backBufferDesc.Height;


		device->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, swapchainRTV.GetAddressOf());
		renderTargetViews.push_back(swapchainRTV.Get());
		//Shared<D3D11Texture2D> backBufferTexture = MakeShared<D3D11Texture2D>(device, backBuffer);
		//colorAttachments.push_back(swapchainRTV.Get());
		//DAYDREAM_CORE_ASSERT(backBuffer, "Backbuffer is nullptr!");

		CreateAttachments();
	}
	D3D11Framebuffer::~D3D11Framebuffer()
	{
		device = nullptr;
		colorAttachments.clear();
		renderTargetViews.clear();
		depthStencilView = nullptr;
		depthAttachment = nullptr;
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

	Shared<Texture2D> Daydream::D3D11Framebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		DAYDREAM_CORE_ASSERT(_index < colorAttachments.size(), "index out of range");
		return colorAttachments[_index];
	}
	void D3D11Framebuffer::Resize(UInt32 _width, UInt32 _height)
	{
		width = _width;
		height = _height;

		colorAttachments.clear();
		renderTargetViews.clear();
		depthStencilView = nullptr;
		depthAttachment = nullptr;

		CreateAttachments();
	}
	//Shared<Texture2D> D3D11Framebuffer::GetDepthAttachemntTexture(UInt32 _index)
	//{
	//	if (depthAttachment == nullptr)
	//	{
	//		DAYDREAM_CORE_WARN("depthAttachment is nullptr");
	//	}
	//	return depthAttachment;
	//}
}