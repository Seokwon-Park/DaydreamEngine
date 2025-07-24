#include "SteinsPCH.h"
#include "RenderCommand.h"

namespace Steins
{
	Shared<RenderContext> RenderCommand::context = nullptr;

	void RenderCommand::Init(RenderDevice* _device)
	{
		context = RenderContext::Create(_device);
		if (context)
		{
			context->Init();
		}
	}
}
