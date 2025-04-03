#pragma once

#include "RendererAPI.h"

namespace Steins
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const Color& _color)
		{
			rendererAPI->SetClearColor(_color);
		}

		inline static void Clear()
		{
			rendererAPI->Clear();
		}

		//inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		//{
		//	rendererAPI->DrawIndexed(vertexArray);
		//}

		static void SetRendererAPI(RendererAPI* _rendererAPI) { rendererAPI = _rendererAPI; }
	private:
		static RendererAPI* rendererAPI;
	};
}

