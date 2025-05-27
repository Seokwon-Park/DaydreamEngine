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
	Shared<Framebuffer> Framebuffer::Create(const FramebufferDesc& _spec)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_spec);
	}
}