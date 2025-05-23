#pragma once

namespace Steins
{
	class CommandList
	{
	public:
		// Constrcuter Destructer
		CommandList();
		~CommandList();

		// Delete Function
		CommandList(const CommandList& _Other) = delete;
		CommandList(CommandList&& _Other) noexcept = delete;
		CommandList& operator=(const CommandList& _Other) = delete;
		CommandList& operator=(CommandList&& _Other) noexcept = delete;
	protected:

	private:

	};
}
