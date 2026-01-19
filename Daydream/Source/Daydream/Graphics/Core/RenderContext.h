#pragma once
#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"

namespace Daydream
{
	class RenderDevice;

	class RenderContext
	{
	public:
		virtual void Begin() = 0;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) = 0;
		virtual void SetClearColor(const Color& _color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex = 0, UInt32 _baseVertex = 0) = 0;

	protected:
	private:
	};
}