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
		case RendererAPIType::None:break;
		case RendererAPIType::OpenGL: return new OpenGLRendererAPI();
		case RendererAPIType::DirectX11:return new D3D11RendererAPI(_device);
		case RendererAPIType::DirectX12:return new D3D12RendererAPI(Cast<D3D12GraphicsDevice>(_device));
		case RendererAPIType::Vulkan: return new VulkanRendererAPI(_device);
		case RendererAPIType::Metal:
		default:return nullptr;
		}
		return nullptr;

	}
}
