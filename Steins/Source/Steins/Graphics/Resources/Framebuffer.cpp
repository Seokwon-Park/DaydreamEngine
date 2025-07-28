#include "SteinsPCH.h"
#include "Steins/Graphics/Resources/Framebuffer.h"

#include "Steins/Graphics/Core/RenderDevice.h"
#include "Steins/Graphics/Core/Renderer.h"

namespace Steins
{
	Shared<Framebuffer> Framebuffer::Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_renderPass, _desc);
	}
}