#pragma once

#include "Steins/Render/RendererAPI.h"

namespace Steins
{
	class D3D11RendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

	private:
		Color clearColor = Color();
	};
}

