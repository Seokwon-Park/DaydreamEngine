#include "DaydreamPCH.h"
#include "Swapchain.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Core/Window.h"
#include "Daydream/Core/Application.h"

namespace Daydream
{
    Shared<Swapchain> Swapchain::Create(DaydreamWindow* _window, const SwapchainDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateSwapchain(_window, _desc);
    }
}
