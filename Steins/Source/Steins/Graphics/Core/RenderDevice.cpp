#include "SteinsPCH.h"
#include "Steins/Graphics/Core/RenderDevice.h"

#include "Steins/Core/Window.h"

#include "Steins/Graphics/Core/SwapChain.h"

#include "Steins/Graphics/API/OpenGL/OpenGLRenderDevice.h"
#include "Steins/Graphics/API/DirectX11/D3D11RenderDevice.h"
#include "Steins/Graphics/API/DirectX12/D3D12RenderDevice.h"
#include "Steins/Graphics/API/Vulkan/VulkanRenderDevice.h"

namespace Steins
{
	Unique<RenderDevice> RenderDevice::Create(RendererAPIType _API)
	{
		switch (_API)
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return MakeUnique<OpenGLRenderDevice>();
		case RendererAPIType::DirectX11: return MakeUnique<D3D11RenderDevice>();
		case RendererAPIType::DirectX12: return MakeUnique<D3D12RenderDevice>();
		case RendererAPIType::Vulkan: return MakeUnique<VulkanRenderDevice>();
		default: return nullptr;
		}
	}
	void RenderDevice::CreateSwapChainForWnd(SteinsWindow* _window)
	{
		SwapChainSpecification desc;
		desc.width = _window->GetWidth();
		desc.height = _window->GetHeight();
		desc.bufferCount = 2;
		desc.format = RenderFormat::R8G8B8A8_UNORM;
		desc.isFullscreen = false;
		desc.isVSync = _window->IsVSync();

		Shared<SwapChain> swapchain = CreateSwapChain(&desc, _window);
		_window->SetSwapChain(swapchain);
	}
}
