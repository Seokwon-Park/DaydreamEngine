#pragma once

#include "Steins/Render/RendererAPI.h"

namespace Steins
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;

		void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

		//virtual void DrawIndexed(const std::shared_ptr<Vertexstd::vector>& vertexstd::vector) override;
	};
}

