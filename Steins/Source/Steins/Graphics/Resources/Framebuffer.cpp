#include "SteinsPCH.h"
#include "Steins/Graphics/Resources/Framebuffer.h"

#include "Steins/Graphics/Core/RenderDevice.h"
#include "Steins/Graphics/Core/Renderer.h"

#include "Steins/Core/Application.h"


#include "Steins/Graphics/API/OpenGL/OpenGLFramebuffer.h"
#include "Steins/Graphics/API/DirectX11/D3D11Framebuffer.h"
#include "Steins/Graphics/API/OpenGL/OpenGLFramebuffer.h"
#include "Steins/Graphics/API/OpenGL/OpenGLFramebuffer.h"
#include "Steins/Graphics/API/OpenGL/OpenGLFramebuffer.h"

namespace Steins
{
	Shared<Framebuffer> Framebuffer::Create(const FramebufferDesc& _spec)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_spec);
	}
}