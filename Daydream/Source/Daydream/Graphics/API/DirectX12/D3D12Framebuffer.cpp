#include "DaydreamPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12Swapchain.h"

namespace Daydream
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;
		renderPass = _renderPass;

		swapchainRTVHandle.ptr = 0;
		
		CreateAttachments();
	}

	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, D3D12Swapchain* _swapChain, ID3D12Resource* _swapchainImage)
	{
		device = _device;
		auto desc = _swapchainImage->GetDesc();
		width = (UInt32)desc.Width;
		height = (UInt32)desc.Height;
		renderPass = _renderPass;

		D3D12_RESOURCE_DESC textureDesc = _swapchainImage->GetDesc();

		device->GetRTVHeapAlloc().Alloc(&swapchainRTVHandle);
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		device->GetDevice()->CreateRenderTargetView(_swapchainImage, &rtvDesc, swapchainRTVHandle);

		renderTargetHandles.push_back(swapchainRTVHandle);

		CreateAttachments();
	}

	D3D12Framebuffer::~D3D12Framebuffer()
	{
		if (swapchainRTVHandle.ptr != 0)
		{
			device->GetRTVHeapAlloc().Free(swapchainRTVHandle);
		}
		renderTargetHandles.clear();
		depthStencilHandle;
		colorAttachments.clear();
		depthAttachment = nullptr;
		device = nullptr;
	}

	Shared<Texture2D> D3D12Framebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index];
	}

	void D3D12Framebuffer::Resize(UInt32 _width, UInt32 _height)
	{
		width = _width;
		height = _height;

		oldAttachments.clear();
		oldAttachments = colorAttachments;
		colorAttachments.clear();
		oldAttachments.push_back(depthAttachment);
		depthAttachment = nullptr;
		renderTargetHandles.clear();
		depthStencilHandle.ptr = 0;

		CreateAttachments();
	}

	void D3D12Framebuffer::CreateAttachments()
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

			Shared<D3D12Texture2D> colorTexture = MakeShared<D3D12Texture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			renderTargetHandles.push_back(colorTexture->GetRTVCPUHandle());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil;

			Shared<D3D12Texture2D> depthTexture = MakeShared<D3D12Texture2D>(device, textureDesc);
			depthAttachment = depthTexture;
			depthStencilHandle = depthTexture->GetDSVCPUHandle();
		}
	}
}