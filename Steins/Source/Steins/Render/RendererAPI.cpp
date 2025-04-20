#include "SteinsPCH.h"
#include "RendererAPI.h"

#include "Platform/RenderSystem/DirectX11/D3D11RendererAPI.h"
#include "Platform/RenderSystem/DirectX12/D3D12RendererAPI.h"
#include "Platform/RenderSystem/OpenGL/OpenGLRendererAPI.h"
#include "Platform/RenderSystem/Vulkan/VulkanRendererAPI.h"

namespace Steins
{
	RendererAPI* RendererAPI::Create(GraphicsDevice* _device)
	{
		switch (_device->GetAPI())
		{
		case Steins::RendererAPIType::None:break;
		case Steins::RendererAPIType::OpenGL: return new OpenGLRendererAPI();
		case Steins::RendererAPIType::DirectX11:return new D3D11RendererAPI(_device);
		case Steins::RendererAPIType::DirectX12:return new D3D12RendererAPI(_device);
		case Steins::RendererAPIType::Vulkan: return new VulkanRendererAPI(_device);
		case Steins::RendererAPIType::Metal:
		default:return nullptr;
		}
		return nullptr;

	}
}
