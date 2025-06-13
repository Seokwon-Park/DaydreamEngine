#pragma once

#include "Steins/Graphics/Core/GraphicsContext.h"

#include "D3D12RenderDevice.h"

namespace Steins
{
	class D3D12GraphicsContext: public GraphicsContext
	{
	public:
		D3D12GraphicsContext(D3D12RenderDevice* _device);

		virtual void Init() override;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) override {};
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;
	private:
		D3D12RenderDevice* device;
	};
}

