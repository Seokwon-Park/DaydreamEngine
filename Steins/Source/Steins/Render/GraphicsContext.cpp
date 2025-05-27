#include "SteinsPCH.h"
#include "GraphicsContext.h"

#include "Steins/Render/Renderer.h"

namespace Steins
{
	Shared<GraphicsContext> GraphicsContext::Create(GraphicsDevice* _device)
	{
		return Renderer::GetRenderDevice()->CreateContext();

	}
}
