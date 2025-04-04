#pragma once

#include "GraphicsDevice.h"

#include "RenderCommand.h"

namespace Steins
{
	class Renderer
	{
	public:
		static void Init(GraphicsDevice* _device);
		
		static void BeginScene();
		static void EndScene();

		static void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex);

		inline static RendererAPIType GetAPI() { return RendererAPI::GetRendererAPI(); }
	};
}


