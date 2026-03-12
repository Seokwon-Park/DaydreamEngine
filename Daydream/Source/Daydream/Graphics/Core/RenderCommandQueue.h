#pragma once

namespace Daydream
{
	class RenderCommandQueue
	{
	public:
		RenderCommandQueue() {};
		~RenderCommandQueue() { commandList.clear(); };

		void AddCommand(FunctionPtr<void()> _command);
		void Execute();
	protected:

	private:
		Array<FunctionPtr<void()>> commandList;
	};
}
