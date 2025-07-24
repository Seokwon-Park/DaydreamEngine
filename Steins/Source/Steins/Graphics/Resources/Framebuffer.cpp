#include "SteinsPCH.h"
#include "Steins/Graphics/Resources/Framebuffer.h"

#include "Steins/Graphics/Core/RenderDevice.h"
#include "Steins/Graphics/Core/Renderer.h"

namespace Steins
{
	Shared<Framebuffer> Framebuffer::Create(const FramebufferDesc& _spec)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_spec);
	}
}