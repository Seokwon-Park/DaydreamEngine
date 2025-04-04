#pragma once

#include "Base/MathTypes.h"
#include "Steins/Enum/RendererAPIType.h"

namespace Steins
{
	class GraphicsDevice
	{
	public:
		enum class PrimitiveTopology
		{
			PointList,
			LineList,
			LineStrip,
			TriangleList,
			TriangleStrip
		};

		GraphicsDevice(class SteinsWindow* _window);
		virtual ~GraphicsDevice() {};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Render() = 0;
		virtual void SwapBuffers() = 0;

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) = 0;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) = 0;

		static GraphicsDevice* Create(class SteinsWindow* _window);
	protected:
		class SteinsWindow* windowHandle;
	};
}
