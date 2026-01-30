#pragma once

#include "Daydream/Graphics/Core/RenderContext.h"

#include "D3D12RenderDevice.h"

namespace Daydream
{
	class D3D12GraphicsContext : public RenderContext
	{
	public:
		D3D12GraphicsContext(D3D12RenderDevice* _device, UInt32 _framesInFlight);
		virtual ~D3D12GraphicsContext() {};

		virtual void BeginCommandList() override;
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) override {};
		virtual void SetClearColor(const Color& _color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;
	private:
		D3D12RenderDevice* device;

		Array<ComPtr<ID3D12GraphicsCommandList>> commandLists;
		Array<ComPtr<ID3D12CommandAllocator>> commandAllocators;
	};
}

