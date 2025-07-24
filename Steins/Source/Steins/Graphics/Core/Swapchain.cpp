#include "SteinsPCH.h"
#include "Swapchain.h"

#include "Steins/Graphics/Core/Renderer.h"
#include "Steins/Core/Window.h"
#include "Steins/Core/Application.h"

namespace Steins
{
    Shared<Swapchain> Swapchain::Create(SteinsWindow* _window, const SwapchainDesc& _desc)
    {
        return Renderer::GetRenderDevice()->CreateSwapchain(_window, _desc);
    }
}
