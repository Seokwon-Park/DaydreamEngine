#pragma once

namespace Daydream
{
	class RenderCommandList
	{
	public:
		RenderCommandList();
		virtual ~RenderCommandList();

		virtual void Begin(){};
		virtual void End() {};
		virtual void WaitForCompletion() {}
	protected:

	private:

	};
}
