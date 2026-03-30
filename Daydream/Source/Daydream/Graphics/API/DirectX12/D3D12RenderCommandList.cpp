#include "DaydreamPCH.h"
#include "D3D12RenderCommandList.h"

namespace Daydream
{
	D3D12RenderCommandList::D3D12RenderCommandList(D3D12RenderDevice* _device)
	{
		device = _device;

		HRESULT hr = device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Command Allocator!");

		hr = device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create CommandList!");

		commandList->Close();
	}

	D3D12RenderCommandList::~D3D12RenderCommandList()
	{
	}
	void D3D12RenderCommandList::Begin()
	{
		commandAllocator->Reset();
		commandList->Reset(commandAllocator.Get(), nullptr);

		ID3D12DescriptorHeap* heaps[] = { device->GetCBVSRVUAVHeap(), device->GetSamplerHeap() };
		commandList->SetDescriptorHeaps(2, heaps);
	}
	void D3D12RenderCommandList::End()
	{
		commandList->Close();
	}
	void D3D12RenderCommandList::WaitForCompletion()
	{
		Array<ID3D12CommandList*> execCommandLists = { commandList.Get() };
		device->GetCommandQueue()->ExecuteCommandLists((UInt32)execCommandLists.size(), execCommandLists.data());

		// 5. Fence를 사용하여 작업이 완료될 때까지 대기
		uploadFenceValue++;
		commandQueue->Signal(uploadFence.Get(), uploadFenceValue);

		if (uploadFence->GetCompletedValue() < uploadFenceValue)
		{
			uploadFence->SetEventOnCompletion(uploadFenceValue, uploadFenceEvent);
			WaitForSingleObject(uploadFenceEvent, INFINITE);
		}
	}
}

