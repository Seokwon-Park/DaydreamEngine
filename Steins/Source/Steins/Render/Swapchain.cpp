#include "SteinsPCH.h"
#include "SwapChain.h"

#include "Renderer.h"
#include "Steins/Core/Window.h"
#include "Steins/Core/Application.h"
#include "Platform/RenderSystem/DirectX12/D3D12Swapchain.h"
#include "Platform/RenderSystem/DirectX11/D3D11Swapchain.h"
#include "Platform/RenderSystem/OpenGL/OpenGLSwapchain.h"
#include "Platform/RenderSystem/Vulkan/VulkanSwapchain.h"

namespace Steins
{
    Shared<SwapChain> SwapChain::Create(SwapChainSpecification* _desc, SteinsWindow* _window, RendererAPIType _type)
    {
		switch (_type)
		{
		case Steins::RendererAPIType::None:break;
		case Steins::RendererAPIType::OpenGL:return MakeShared<OpenGLSwapchain>(_desc, _window);
		case Steins::RendererAPIType::DirectX11: return MakeShared<D3D11SwapChain>(Application::GetGraphicsDevice(), _desc, _window);
		case Steins::RendererAPIType::DirectX12: return MakeShared<D3D12SwapChain>(Application::GetGraphicsDevice(), _desc, _window);
		case Steins::RendererAPIType::Vulkan: return MakeShared<VulkanSwapChain>(Application::GetGraphicsDevice(), _desc, _window);
		//case Steins::RendererAPIType::Metal:break;
		default:
			break;
		}
		STEINS_CORE_ERROR("Failed to create swapchain!");
		return nullptr;
    }
}
