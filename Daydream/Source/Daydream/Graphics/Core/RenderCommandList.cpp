#include "DaydreamPCH.h"
#include "RenderCommandList.h"

namespace Daydream
{
	void RenderCommandList::AddCommand(FunctionPtr<void()> _command)
	{
		commandList.push_back(_command);
	}

	void RenderCommandList::Execute()
	{
		for (FunctionPtr command : commandList)
		{
			command();
		}
	}
}

