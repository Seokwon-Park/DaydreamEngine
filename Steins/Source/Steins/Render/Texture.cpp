#include "SteinsPCH.h"
#include "Texture.h"

#include "Steins/Render/Renderer.h"

namespace Steins
{
    Shared<Texture2D> Texture2D::Create(const FilePath& _path)
    {
		switch (Renderer::GetAPI())
		{
		case Steins::RendererAPIType::None: return nullptr;
		case Steins::RendererAPIType::OpenGL: return nullptr;
		case Steins::RendererAPIType::DirectX11: return nullptr;
		case Steins::RendererAPIType::DirectX12: return nullptr;
		case Steins::RendererAPIType::Vulkan: return nullptr;
		case Steins::RendererAPIType::Metal: return nullptr;
		default:
			break;
		}
		return nullptr;
    }
}