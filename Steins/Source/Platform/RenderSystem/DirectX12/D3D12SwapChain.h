#pragma once

#include "Steins/Render/Swapchain.h"
#include "D3D12GraphicsDevice.h"
#include "D3D12Framebuffer.h"


namespace Steins
{
	class D3D12SwapChain : public SwapChain
	{
	public:
		D3D12SwapChain(GraphicsDevice* _device, SwapChainSpecification* _desc, SteinsWindow* _window);
		virtual ~D3D12SwapChain() override;

		// Inherited via Swapchain
		void SetVSync(bool _enabled) override;
		void SwapBuffers() override;

		void WaitForGPU();
		void MoveToNextFrame();

		inline IDXGISwapChain3* GetDXGISwapChain() { return swapChain.Get(); }
	private:
		D3D12GraphicsDevice* device;
		ComPtr<IDXGISwapChain3> swapChain;

		Int32 frameIndex = 0;
		ComPtr<ID3D12Fence> fence;
		UINT64 fenceValues[2];
		HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu
	};
}