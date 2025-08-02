#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

#include "Daydream/Graphics/Core/RenderDevice.h"
#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Shared<Framebuffer> Framebuffer::Create(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateFramebuffer(_renderPass, _desc);
	}
}