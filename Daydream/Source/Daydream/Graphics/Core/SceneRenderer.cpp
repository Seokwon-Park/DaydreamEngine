#include "DaydreamPCH.h"
#include "SceneRenderer.h"

namespace Daydream
{
	SceneRenderer::SceneRenderer()
	{
		renderGraph = MakeShared<RenderGraph>();
	}

	SceneRenderer::~SceneRenderer()
	{
	}
}

