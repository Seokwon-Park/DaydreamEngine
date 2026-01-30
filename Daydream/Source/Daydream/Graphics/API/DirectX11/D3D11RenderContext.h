#pragma once

#include "Daydream/Graphics/Core/RenderContext.h"
#include "D3D11RenderDevice.h"

namespace Daydream
{
	class D3D11GraphicsContext : public RenderContext
	{
	public:
		D3D11GraphicsContext(D3D11RenderDevice* _device);

		virtual void BeginCommandList() override;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) override;
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

	private:
		D3D11RenderDevice* device;
		Color clearColor = Color();
	};
}

