#pragma once

#include "Steins/Render/Swapchain.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11SwapChain : public SwapChain
	{
	public:
		D3D11SwapChain(GraphicsDevice* _device, SwapChainDesc* _desc, SteinsWindow* _window);
		virtual ~D3D11SwapChain();

		virtual void SetVSync(bool _enabled);
		virtual void SwapBuffers() override;
	private:
		D3D11GraphicsDevice* device;
		ComPtr<IDXGISwapChain> swapChain;
	};
}