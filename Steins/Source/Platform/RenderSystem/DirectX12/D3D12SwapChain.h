#pragma once

#include "Steins/Render/Swapchain.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12SwapChain : public SwapChain
	{
	public:
		D3D12SwapChain(GraphicsDevice* _device, SwapchainDesc* _desc, SteinsWindow* _window);
		virtual ~D3D12SwapChain() override;

		// Inherited via Swapchain
		void SetVSync(bool _enabled) override;
		void SwapBuffers() override;
	private:
		D3D12GraphicsDevice* device;
		ComPtr<IDXGISwapChain3> swapChain;

		ComPtr<ID3D12Fence> fences[2];
		UINT fenceValues[2];
	};
}