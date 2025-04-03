#pragma once

#include "Steins/Window.h"

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

		GraphicsDevice(SteinsWindow* _window) : windowHandle(_window) {}
		virtual ~GraphicsDevice() {};

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Render() = 0;
		virtual void SwapBuffers() = 0;

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) = 0;

	protected:
		SteinsWindow* windowHandle;
	};
}
