#include "DaydreamPCH.h"
#include "RenderCommandQueue.h"

namespace Daydream
{
	void RenderCommandQueue::AddCommand(FunctionPtr<void()> _command)
	{
		commandList.push_back(_command);
	}

	void RenderCommandQueue::Execute()
	{
		for (FunctionPtr command : commandList)
		{
			command();
		}
	}
}

