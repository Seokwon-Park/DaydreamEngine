#pragma once

#include "Daydream/Graphics/Core/RenderCommandList.h"
#include "D3D12RenderDevice.h"

namespace Daydream
{
	class D3D12RenderCommandList : public RenderCommandList
	{
	public:
		D3D12RenderCommandList(D3D12RenderDevice* _device);
		~D3D12RenderCommandList();

		virtual void Begin() override;
		virtual void End() override;
		virtual void WaitForCompletion() override;

		ID3D12GraphicsCommandList* GetID3D12GraphicsCommandList() const { return commandList.Get(); }
		ID3D12CommandAllocator* GetID3D12Allocator() const { return commandAllocator.Get(); }
	protected:

	private:
		D3D12RenderDevice* device;

		ComPtr<ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
	};
}
