#include "SteinsPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12SwapChain.h"

namespace Steins
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12GraphicsDevice* _device, const FramebufferDesc& _spec)
	{
	}
	D3D12Framebuffer::D3D12Framebuffer(D3D12GraphicsDevice* _device, D3D12SwapChain* _swapChain)
	{
		device = _device;
		swapChain = _swapChain;

		renderTargets.resize(2);
		IDXGISwapChain* dxgiSwapChain = _swapChain->GetDXGISwapChain();
		for (int i = 0; i < 2; i++)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
			device->GetRTVHeapAlloc().AllocCPU(&cpuHandle);

			dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].GetAddressOf()));
			STEINS_CORE_ASSERT(renderTargets[i].Get(), "Backbuffer is nullptr!");
			device->GetDevice()->CreateRenderTargetView(renderTargets[i].Get(), nullptr, cpuHandle);
			renderTargetHandles.push_back(cpuHandle);
		}
	}
	void D3D12Framebuffer::Bind() const
	{
		device->GetCommandList()->OMSetRenderTargets(1, &renderTargetHandles[frameIndex], false, nullptr);
	}
	void D3D12Framebuffer::Clear(Color _color)
	{
		device->GetCommandList()->ClearRenderTargetView(renderTargetHandles[frameIndex], _color.color, 0, nullptr);
	}
}