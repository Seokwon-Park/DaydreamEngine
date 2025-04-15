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

		virtual void Clear() = 0;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) = 0;

		static GraphicsDevice* Create();
	protected:
		class SteinsWindow* windowHandle;
	};
}
