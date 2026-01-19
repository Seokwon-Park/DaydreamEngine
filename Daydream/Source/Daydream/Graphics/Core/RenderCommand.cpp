#include "DaydreamPCH.h"
#include "RenderCommand.h"

namespace Daydream
{
	Shared<RenderContext> RenderCommand::context = nullptr;

	//void RenderCommand::Init(RenderDevice* _device)
	//{
	//	context = RenderContext::Create(_device);
	//	if (context)
	//	{
	//		context->Begin();
	//	}
	//}
}
