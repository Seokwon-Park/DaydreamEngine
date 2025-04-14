#include "SteinsPCH.h"
#include "D3D11Swapchain.h"

namespace Steins
{
	D3D11Swapchain::D3D11Swapchain(GraphicsDevice* _device, SwapchainDesc* _desc, SteinsWindow* _window)
	{

	}
	void D3D11Swapchain::SetVSync(bool _enabled)
	{
	}
	void D3D11Swapchain::SwapBuffers()
	{
		swapchain->Present(desc.isVSync, 0);
	}
}