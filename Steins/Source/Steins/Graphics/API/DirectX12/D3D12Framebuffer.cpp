#include "SteinsPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12Swapchain.h"

namespace Steins
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, D3D12RenderPass* _renderPass, const FramebufferDesc& _desc)
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

			Shared<D3D12Texture2D> colorTexture = MakeShared<D3D12Texture2D>(device, textureDesc);
			colorAttachments.push_back(colorTexture);
			renderTargetHandles.push_back(colorTexture->GetRTVCPUHandle());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = _desc.width;
			textureDesc.height = _desc.height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil;

			Shared<D3D12Texture2D> depthTexture = MakeShared<D3D12Texture2D>(device, textureDesc);
			depthAttachment = depthTexture;
			depthStencilHandle = depthTexture->GetDSVCPUHandle();
		}

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

	}
	void D3D12Framebuffer::Begin() const
	{
		Array<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles;
		for (auto texture : colorAttachments)
		{
			renderTargetHandles.push_back(texture->GetRTVCPUHandle());
		}
		device->GetCommandList()->OMSetRenderTargets((UInt32)renderTargetHandles.size(), renderTargetHandles.data(), false, nullptr);
	}
	void D3D12Framebuffer::Clear(Color _color)
	{
		for (auto texture : colorAttachments)
		{
			device->GetCommandList()->ClearRenderTargetView(texture->GetRTVCPUHandle(), _color.color, 0, nullptr);
		}
	}
	void* D3D12Framebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return reinterpret_cast<void*>(colorAttachments[_index]->GetSRVGPUHandle().ptr);
	}
}