#pragma once

#include "Steins/Render/RendererAPI.h"

#include "D3D12GraphicsDevice.h"

namespace Steins
{
	class D3D12RendererAPI : public RendererAPI
	{
	public:
		D3D12RendererAPI(GraphicsDevice* _device);
		void SetClearColor(const Color& _color) override;
		void Clear() override;
		void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;
	private:
		D3D12GraphicsDevice* device;
	};
}

