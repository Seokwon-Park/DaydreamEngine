#include "DaydreamPCH.h"
#include "RenderCommandQueue.h"

namespace Daydream
{
	void RenderCommandQueue::AddRenderCommand(RenderCommand _command)
	{
		renderCommandQueue.push(_command);
	}

	void RenderCommandQueue::Execute()
	{
		while(!renderCommandQueue.empty())
		{
			renderCommandQueue.front()();
			renderCommandQueue.pop();
		}
	}
}

