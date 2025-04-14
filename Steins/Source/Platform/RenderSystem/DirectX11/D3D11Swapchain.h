#pragma once

#include "Steins/Render/Swapchain.h"

namespace Steins
{
	class D3D11Swapchain : public Swapchain
	{
	public:
		D3D11Swapchain(SwapchainDesc* _desc);
		virtual ~D3D11Swapchain() = default;

		virtual void SetVSync(bool _enabled);
	private:
		ComPtr<IDXGISwapChain> swapchain;
	};
}