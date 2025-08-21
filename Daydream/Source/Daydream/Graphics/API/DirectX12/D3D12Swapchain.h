#pragma once

#include "Daydream/Graphics/Core/Swapchain.h"
#include "D3D12RenderDevice.h"
#include "D3D12Framebuffer.h"


namespace Daydream
{
	class D3D12Swapchain : public Swapchain
	{
	public:
		D3D12Swapchain(RenderDevice* _device, DaydreamWindow* _window, const SwapchainDesc& _desc);
		virtual ~D3D12Swapchain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapchain(UInt32 _width, UInt32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Shared<Framebuffer> GetBackFramebuffer() { return framebuffers[frameIndex]; };

		void WaitForGPU();
		void MoveToNextFrame();

		UInt32 GetBackbufferIndex() { return frameIndex; }

		inline IDXGISwapChain3* GetDXGISwapchain() { return swapchain.Get(); }
	private:
		D3D12RenderDevice* device;
		ComPtr<IDXGISwapChain3> swapchain;
		Array<Shared<D3D12Framebuffer>> framebuffers;
		Array<Shared<D3D12Framebuffer>> oldFramebuffers;
		Array<ComPtr<ID3D12GraphicsCommandList>> commandLists;
		Array<ComPtr<ID3D12CommandAllocator>> commandAllocators;

		Int32 frameIndex = 0;
		UInt32 bufferCount = 0;
		DXGI_FORMAT format;

		Array<ComPtr<ID3D12Resource>> backBuffers;

		ComPtr<ID3D12Fence> fence;
		Array<UINT64> fenceValues;
		Wrappers::Event fenceEvent;

		HWND windowHandle;
	};
}