#pragma once

#include "Steins/Render/RendererAPI.h"

namespace Steins
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const Color& color) override;
		virtual void Clear() override;

		//virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}

