#include "SteinsPCH.h"
#include "RenderPass.h"

#include "Steins/Graphics/Core/Renderer.h"
#include "Steins/Graphics/Resources/Framebuffer.h"

namespace Steins
{
	Shared<RenderPass> RenderPass::Create(const RenderPassDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateRenderPass(_desc);
	}
}