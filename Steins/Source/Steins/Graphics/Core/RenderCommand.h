#pragma once

#include "GraphicsContext.h"

namespace Steins
{
	class RenderCommand
	{
	public:
		static void Init(RenderDevice* _device);

		inline static void SetViewport(UInt32 _width, UInt32 _height)
		{
			context->SetViewport(0, 0, _width, _height);
		}

		inline static void SetClearColor(const Color& _color)
		{
			context->SetClearColor(_color);
		}


		inline static void Clear()
		{
			context->Clear();
		}

		inline static void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex = 0, UInt32 _baseVertex = 0)
		{
			context->DrawIndexed(_indexCount, _startIndex, _baseVertex);
		}

	private:
		static Shared<GraphicsContext> context;
	};
}

