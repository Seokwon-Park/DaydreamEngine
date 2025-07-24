#include "SteinsPCH.h"

#include "Steins/Graphics/Core/Renderer.h"

namespace Steins
{
	Shared<RenderContext> RenderContext::Create(RenderDevice* _device)
	{
		return Renderer::GetRenderDevice()->CreateContext();

	}
}
