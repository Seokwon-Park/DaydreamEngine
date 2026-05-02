#include "DaydreamPCH.h"
#include "Swapchain.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Core/Window.h"
#include "Daydream/Core/Application.h"

namespace Daydream
{
	Shared<Swapchain> Swapchain::Create(const DaydreamWindow& _window, const SwapchainDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateSwapchain(_window, _desc);
    }
	void Swapchain::ResizeSwapchain(UInt32 _width, UInt32 _height)
	{
		isSwapchainResized = true;
		desc.width = _width;
		desc.height = _height;
	}
	Swapchain::Swapchain(const SwapchainDesc& _desc)
	{
		desc = _desc;
	}
}
