#include "SteinsPCH.h"
#include "GraphicsDevice.h"

#include "Steins/Core/Window.h"

#include "Steins/Render/SwapChain.h"

#include "Platform/RenderSystem/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/RenderSystem/DirectX11/D3D11GraphicsDevice.h"
#include "Platform/RenderSystem/DirectX12/D3D12GraphicsDevice.h"
#include "Platform/RenderSystem/Vulkan/VulkanGraphicsDevice.h"

namespace Steins
{
	Unique<GraphicsDevice> GraphicsDevice::Create(RendererAPIType _API)
	{
		switch (_API)
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return MakeUnique<OpenGLGraphicsDevice>();
		case RendererAPIType::DirectX11: return MakeUnique<D3D11GraphicsDevice>();
		case RendererAPIType::DirectX12: return MakeUnique<D3D12GraphicsDevice>();
		case RendererAPIType::Vulkan: return MakeUnique<VulkanGraphicsDevice>();
		default: return nullptr;
		}
	}
	void GraphicsDevice::CreateSwapChainForWnd(SteinsWindow* _window)
	{
		SwapChainSpecification desc;
		desc.width = _window->GetWidth();
		desc.height = _window->GetHeight();
		desc.bufferCount = 2;
		desc.format = RenderFormat::R8G8B8A8_UNORM;
		desc.isFullscreen = false;
		desc.isVSync = _window->IsVSync();

		Shared<SwapChain> swapchain = CreateSwapChain(&desc, _window);
		_window->SetSwapchain(swapchain);
	}
}
