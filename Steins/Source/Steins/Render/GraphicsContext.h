#pragma once
#include "Steins/Enum/RendererEnums.h"
#include "Framebuffer.h"

namespace Steins
{
	class RenderDevice;

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) = 0;
		virtual void SetClearColor(const Color& _color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) = 0;
		virtual void SetFramebuffer(Shared<Framebuffer> _framebuffer)
		{
			currentFramebuffer = _framebuffer;
			_framebuffer->Begin();
		}

		static Shared<GraphicsContext> Create(RenderDevice* _device);
	protected:
	private:
		Shared<Framebuffer> currentFramebuffer;
	};
}