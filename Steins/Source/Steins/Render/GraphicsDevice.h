#pragma once

#include "Base/MathTypes.h"
#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	class GraphicsDevice
	{
	public:
		virtual ~GraphicsDevice() {};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Render() = 0;
		virtual void SwapBuffers() = 0;

		static Unique<GraphicsDevice> Create();
	protected:
		class SteinsWindow* windowHandle;
	};
}
