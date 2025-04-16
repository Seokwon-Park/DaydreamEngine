#pragma once

#include "Steins/Render/RendererAPI.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11RendererAPI : public RendererAPI
	{
	public:
		D3D11RendererAPI(GraphicsDevice* _device);
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

	private:
		class D3D11GraphicsDevice* device;
		Color clearColor = Color();
	};
}

