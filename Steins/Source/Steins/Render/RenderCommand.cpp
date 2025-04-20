#include "SteinsPCH.h"
#include "RenderCommand.h"

namespace Steins
{
	RendererAPI* RenderCommand::rendererAPI = nullptr;

	void RenderCommand::Init(GraphicsDevice* _device)
	{
		rendererAPI = RendererAPI::Create(_device);
	}
}
