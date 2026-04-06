#include "DaydreamPCH.h"
#include "Swapchain.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Core/Window.h"
#include "Daydream/Core/Application.h"

namespace Daydream
{
	void Swapchain::ResizeFramebuffers()
	{
		auto& requestArray = Renderer::GetRenderDevice()->GetFramebufferResizeRequest();
		if (!requestArray.empty())
		{
			for (auto& [framebuffer, size] : requestArray)
			{
				framebuffer->Resize((UInt32)size.x, (UInt32)size.y);
			}
			requestArray.clear();
		}
	}
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
}
