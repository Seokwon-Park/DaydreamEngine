#pragma once

namespace Daydream
{
	using RenderCommand = FunctionPtr<void()>;

	class RenderCommandQueue
	{
	public:
		RenderCommandQueue() {};
		~RenderCommandQueue() { while (!renderCommandQueue.empty()) renderCommandQueue.pop(); };

		void AddRenderCommand(RenderCommand _command);
		void Execute();
	protected:

	private:
		Queue<FunctionPtr<void()>> renderCommandQueue;
	};
}
