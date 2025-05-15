#include "SteinsPCH.h"
#include "D3D12RendererAPI.h"

namespace Steins
{
	D3D12RendererAPI::D3D12RendererAPI(D3D12GraphicsDevice* _device)
	{
		device = _device;
	}
	void D3D12RendererAPI::Init()
	{
	}
	void D3D12RendererAPI::SetClearColor(const Color& _color)
	{
	}

	void D3D12RendererAPI::Clear()
	{
	}

	void D3D12RendererAPI::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->GetCommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetCommandList()->DrawIndexedInstanced(_indexCount, 1, _startIndex, _baseVertex, 0);
		//device->GetCommandList()->DrawInstanced(3, 1, 0, 0);
	}
}
