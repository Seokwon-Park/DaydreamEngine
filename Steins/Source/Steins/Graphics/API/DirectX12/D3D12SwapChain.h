#pragma once

#include "Steins/Graphics/Core/SwapChain.h"
#include "D3D12RenderDevice.h"
#include "D3D12Framebuffer.h"


namespace Steins
{
	class D3D12SwapChain : public SwapChain
	{
	public:
		D3D12SwapChain(RenderDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window);
		virtual ~D3D12SwapChain() override;

		virtual void SetVSync(bool _enabled) override;
		virtual void SwapBuffers() override;
		virtual void ResizeSwapChain(UInt32 _width, UInt32 height) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual Framebuffer* GetBackFramebuffer() { return nullptr; };

		void WaitForGPU();
		void MoveToNextFrame();

		UInt32 GetBackbufferIndex() { return frameIndex; }

		inline IDXGISwapChain3* GetDXGISwapChain() { return swapChain.Get(); }
	private:
		D3D12RenderDevice* device;
		ComPtr<IDXGISwapChain3> swapChain;
		std::vector<Shared<D3D12Framebuffer>> framebuffers;
		std::vector<ID3D12CommandList*> commandLists;
		Int32 frameIndex = 0;
		ComPtr<ID3D12Fence> fence;
		std::vector<UINT64> fenceValues;
		HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu
	};
}