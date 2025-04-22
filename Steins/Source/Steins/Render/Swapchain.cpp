#include "SteinsPCH.h"
#include "SwapChain.h"

#include "Renderer.h"
#include "Steins/Core/Window.h"
#include "Steins/Core/Application.h"

namespace Steins
{
    Shared<SwapChain> SwapChain::Create(SwapChainSpecification* _desc, SteinsWindow* _window)
    {
        return Application::GetGraphicsDevice()->CreateSwapChain(_desc, _window);
    }
}
