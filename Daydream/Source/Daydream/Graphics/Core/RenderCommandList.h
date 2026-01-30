#pragma once

namespace Daydream
{
	class RenderCommandList
	{
	public:
		RenderCommandList() {};
		~RenderCommandList() {};

		void AddCommand(FunctionPtr<void()> _command);
		void Execute();
	protected:

	private:
		Array<FunctionPtr<void()>> commandList;
	};
}
