#include "DaydreamPCH.h"

#include "Daydream/Graphics/Core/Renderer.h"

namespace Daydream
{
	Shared<RenderContext> RenderContext::Create(RenderDevice* _device)
	{
		return Renderer::GetRenderDevice()->CreateContext();

	}
}
