#include "SteinsPCH.h"
#include "Framebuffer.h"

#include "GraphicsDevice.h"
#include "Renderer.h"

#include "Steins/Core/Application.h"


#include "Platform/RenderSystem/OpenGL/OpenGLFramebuffer.h"
#include "Platform/RenderSystem/DirectX11/D3D11Framebuffer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLFramebuffer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLFramebuffer.h"
#include "Platform/RenderSystem/OpenGL/OpenGLFramebuffer.h"

namespace Steins
{
	Shared<Framebuffer> Framebuffer::Create(const FramebufferSpecification& _spec)
	{
		switch (Application::GetGraphicsDevice()->GetAPI())
		{
		case RendererAPIType::None: return nullptr;
		case RendererAPIType::OpenGL: return nullptr;
		case RendererAPIType::DirectX11: return MakeShared<D3D11Framebuffer>(Application::GetGraphicsDevice(), _spec) ;
		case RendererAPIType::DirectX12: return nullptr;
		case RendererAPIType::Vulkan: return nullptr;
		case RendererAPIType::Metal: return nullptr;
		default:
			break;
		}
		return Shared<Framebuffer>();
	}
}