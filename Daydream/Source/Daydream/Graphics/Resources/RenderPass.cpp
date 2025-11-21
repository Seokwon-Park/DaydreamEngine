#include "DaydreamPCH.h"
#include "RenderPass.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

namespace Daydream
{
	Shared<RenderPass> RenderPass::Create(const RenderPassDesc& _desc)
	{
		return Renderer::GetRenderDevice()->CreateRenderPass(_desc);
	}
}