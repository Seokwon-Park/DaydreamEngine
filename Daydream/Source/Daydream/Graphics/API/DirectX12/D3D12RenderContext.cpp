#include "DaydreamPCH.h"
#include "D3D12RenderContext.h"

namespace Daydream
{
	D3D12GraphicsContext::D3D12GraphicsContext(D3D12RenderDevice* _device, UInt32 _framesInFlight)
	{
		device = _device;

		commandLists.resize(_framesInFlight);
		commandAllocators.resize(_framesInFlight);
		for (UInt32 i = 0; i < _framesInFlight; i++)
		{
			HRESULT hr = device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocators[i].GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create command allocator {0}", i);

			hr = device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[i].Get(), nullptr, IID_PPV_ARGS(commandLists[i].GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create commandlist");

			commandLists[i]->Close();
		}
	}
	void D3D12GraphicsContext::BeginCommandList()
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
