#pragma once

#include "Steins/Render/Swapchain.h"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	class D3D11Swapchain : public Swapchain
	{
	public:
		D3D11Swapchain(GraphicsDevice* _device, SwapchainDesc* _desc, SteinsWindow* _window);
		virtual ~D3D11Swapchain() = default;

		virtual void SetVSync(bool _enabled);
		virtual void SwapBuffers() override;
	private:
		ComPtr<IDXGISwapChain> swapchain;
	};
}