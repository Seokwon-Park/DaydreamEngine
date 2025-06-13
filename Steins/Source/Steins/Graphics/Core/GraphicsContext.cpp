#include "SteinsPCH.h"
#include "GraphicsContext.h"

#include "Steins/Graphics/Core/Renderer.h"

namespace Steins
{
	Shared<GraphicsContext> GraphicsContext::Create(RenderDevice* _device)
	{
		return Renderer::GetRenderDevice()->CreateContext();

	}
}
