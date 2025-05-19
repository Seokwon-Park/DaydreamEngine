#pragma once

#include "RendererAPI.h"

#include "Platform/RenderSystem/OpenGL/OpenGLRendererAPI.h"
#include "Platform/RenderSystem/DirectX11/D3D11RendererAPI.h"
#include "Platform/RenderSystem/DirectX12/D3D12RendererAPI.h"

namespace Steins
{
	class RenderCommand
	{
	public:
		static void Init(GraphicsDevice* _device);

		inline static void SetViewport(UInt32 _width, UInt32 _height)
		{
			rendererAPI->SetViewport(0, 0, _width, _height);
		}

		inline static void SetClearColor(const Color& _color)
		{
			rendererAPI->SetClearColor(_color);
		}


		inline static void Clear()
		{
			rendererAPI->Clear();
		}

		inline static void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex = 0, UInt32 _baseVertex = 0)
		{
			rendererAPI->DrawIndexed(_indexCount, _startIndex, _baseVertex);
		}

	private:
		static RendererAPI* rendererAPI;
	};
}

