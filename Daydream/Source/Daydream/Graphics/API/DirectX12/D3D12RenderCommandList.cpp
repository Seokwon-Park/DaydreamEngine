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
	}
	void D3D12RenderCommandList::End()
	{
		commandList->Close();
	}
	void D3D12RenderCommandList::WaitForCompletion()
	{

	}
}

