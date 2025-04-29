#include "SteinsPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12SwapChain.h"

namespace Steins
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12GraphicsDevice* _device, const FramebufferSpecification& _spec)
	{
	}
	D3D12Framebuffer::D3D12Framebuffer(D3D12GraphicsDevice* _device, D3D12SwapChain* _swapChain)
	{
		device = _device;
		renderTargets.resize(2);
		IDXGISwapChain* dxgiSwapChain = _swapChain->GetDXGISwapChain();
		UINT rtvDescriptorSize = device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = device->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart();
		for (int i = 0; i < 2; i++)
		{
			dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(renderTargets[i].GetAddressOf()));
			STEINS_CORE_ASSERT(renderTargets[i].Get(), "Backbuffer is nullptr!");
			device->GetDevice()->CreateRenderTargetView(renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.ptr += rtvDescriptorSize;
			device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i]));
		}
	}
	void D3D12Framebuffer::Bind() const
	{
		device->GetCommandList()->OMSetRenderTargets(renderTargetHandles.size(), renderTargetHandles.data(), false, nullptr);
	}
}