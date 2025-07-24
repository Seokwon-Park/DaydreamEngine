#include "SteinsPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12Swapchain.h"

namespace Steins
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, const FramebufferDesc& _spec)
	{
	}
	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, D3D12Swapchain* _swapChain)
	{
		device = _device;
		swapChain = _swapChain;

		renderTargets.resize(1);
		IDXGISwapChain* dxgiSwapchain = _swapChain->GetDXGISwapchain();
		for (int i = 0; i < renderTargets.size(); i++)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
			device->GetRTVHeapAlloc().AllocCPU(&cpuHandle);

			dxgiSwapchain->GetBuffer(_swapChain->GetBackbufferIndex(), IID_PPV_ARGS(renderTargets[i].GetAddressOf()));
			STEINS_CORE_ASSERT(renderTargets[i].Get(), "Backbuffer is nullptr!");
			device->GetDevice()->CreateRenderTargetView(renderTargets[i].Get(), nullptr, cpuHandle);
			renderTargetHandles.push_back(cpuHandle);
		}
	}
	void D3D12Framebuffer::Begin() const
	{
		device->GetCommandList()->OMSetRenderTargets((UInt32)renderTargetHandles.size(), renderTargetHandles.data(), false, nullptr);
	}
	void D3D12Framebuffer::Clear(Color _color)
	{
		for (auto rth : renderTargetHandles)
		{
			device->GetCommandList()->ClearRenderTargetView(rth, _color.color, 0, nullptr);
		}
	}
}