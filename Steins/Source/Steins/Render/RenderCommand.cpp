#include "SteinsPCH.h"
#include "RenderCommand.h"

namespace Steins
{
	Shared<GraphicsContext> RenderCommand::context = nullptr;

	void RenderCommand::Init(GraphicsDevice* _device)
	{
		context = GraphicsContext::Create(_device);
		if (context)
		{
			context->Init();
		}
	}
}
