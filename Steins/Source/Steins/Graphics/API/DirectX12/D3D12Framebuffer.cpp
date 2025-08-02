#include "SteinsPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12Swapchain.h"

namespace Steins
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;
		renderPass = _renderPass;

		CreateAttachments();
	}

	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, D3D12Swapchain* _swapChain)
	{
		device = _device;

		IDXGISwapChain* dxgiSwapchain = _swapChain->GetDXGISwapchain();
		//for (int i = 0; i < renderTargets.size(); i++)
		//{
		//	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		//	device->GetRTVHeapAlloc().Alloc(&cpuHandle);

		//	dxgiSwapchain->GetBuffer(_swapChain->GetBackbufferIndex(), IID_PPV_ARGS(renderTargets[i].GetAddressOf()));
		//	STEINS_CORE_ASSERT(renderTargets[i].Get(), "Backbuffer is nullptr!");
		//	device->GetDevice()->CreateRenderTargetView(renderTargets[i].Get(), nullptr, cpuHandle);
		//	renderTargetHandles.push_back(cpuHandle);
		//}
		ComPtr<ID3D12Resource> backBuffer;
		dxgiSwapchain->GetBuffer(_swapChain->GetBackbufferIndex(), IID_PPV_ARGS(backBuffer.GetAddressOf()));

		Shared<D3D12Texture2D> backBufferTexture = MakeShared<D3D12Texture2D>(device, backBuffer);
		colorAttachments.push_back(backBufferTexture);
		renderTargetHandles.push_back(backBufferTexture->GetRTVCPUHandle());
		STEINS_CORE_ASSERT(backBuffer, "Backbuffer is nullptr!");
	}
	D3D12Framebuffer::~D3D12Framebuffer()
	{
		device = nullptr;
		renderTargetHandles.clear();
		depthStencilHandle;
		colorAttachments.clear();
		depthAttachment = nullptr;
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