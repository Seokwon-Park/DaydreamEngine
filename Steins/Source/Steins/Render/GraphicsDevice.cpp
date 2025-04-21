#include "SteinsPCH.h"
#include "GraphicsDevice.h"

#include "Steins/Core/Window.h"
#include "RendererAPI.h"

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
	Shared<SwapChain> GraphicsDevice::CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		return SwapChain::Create(_desc, _window, API);
	}
}
