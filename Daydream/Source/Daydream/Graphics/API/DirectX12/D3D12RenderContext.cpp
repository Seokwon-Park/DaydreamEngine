#include "DaydreamPCH.h"
#include "D3D12RenderContext.h"

namespace Daydream
{
	D3D12GraphicsContext::D3D12GraphicsContext(D3D12RenderDevice* _device)
	{
		device = _device;
	}
	void D3D12GraphicsContext::Begin()
	{
	}
	void D3D12GraphicsContext::SetClearColor(const Color& _color)
	{
	}

	void D3D12GraphicsContext::Clear()
	{
	}

	void D3D12GraphicsContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->GetCommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetCommandList()->DrawIndexedInstanced(_indexCount, 1, _startIndex, _baseVertex, 0);
		//device->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	}
}
