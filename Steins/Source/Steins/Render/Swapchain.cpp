#include "SteinsPCH.h"
#include "Swapchain.h"

#include "Renderer.h"
#include "Steins/Core/Window.h"
#include "Steins/Core/Application.h"
#include "Platform/RenderSystem/DirectX12/D3D12Swapchain.h"
#include "Platform/RenderSystem/DirectX11/D3D11Swapchain.h"
#include "Platform/RenderSystem/OpenGL/OpenGLSwapchain.h"

namespace Steins
{
    Shared<SwapChain> SwapChain::Create(SwapchainDesc* _desc, SteinsWindow* _window)
    {
		switch (Renderer::GetAPI())
		{
		case Steins::RendererAPIType::None:break;
		case Steins::RendererAPIType::OpenGL:return MakeShared<OpenGLSwapchain>(_desc, _window);
		case Steins::RendererAPIType::DirectX11: return MakeShared<D3D11SwapChain>(Application::GetGraphicsDevice(), _desc, _window);
		case Steins::RendererAPIType::DirectX12: return MakeShared<D3D12SwapChain>(Application::GetGraphicsDevice(), _desc, _window);
		//case Steins::RendererAPIType::Vulkan:break;
		//case Steins::RendererAPIType::Metal:break;
		default:
			break;
		}
    }
}
