#pragma once

#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	struct DescriptorHeapAllocator
	{
		ID3D12DescriptorHeap* heap = nullptr;
		D3D12_DESCRIPTOR_HEAP_TYPE  heapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		D3D12_CPU_DESCRIPTOR_HANDLE heapStartCpu;
		D3D12_GPU_DESCRIPTOR_HANDLE heapStartGpu;
		UINT                        heapHandleIncrement;
		std::vector<int>            freeIndices;

		void Create(ID3D12Device* _device, ID3D12DescriptorHeap* _heap)
		{
			STEINS_CORE_ASSERT(heap == nullptr && freeIndices.empty(), "Can't Create Heap Allocator");
			heap = _heap;
			D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();
			heapType = desc.Type;
			heapStartCpu = heap->GetCPUDescriptorHandleForHeapStart();
			heapStartGpu = heap->GetGPUDescriptorHandleForHeapStart();
			heapHandleIncrement = _device->GetDescriptorHandleIncrementSize(heapType);
			freeIndices.reserve((int)desc.NumDescriptors);
			for (int n = desc.NumDescriptors; n > 0; n--)
				freeIndices.push_back(n - 1);
		}
		void Destroy()
		{
			heap = nullptr;
			freeIndices.clear();
		}
		void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuDescriptorHandle, D3D12_GPU_DESCRIPTOR_HANDLE* _outGpuDescriptorHandle)
		{
			STEINS_CORE_ASSERT(freeIndices.size() > 0, "");
			int idx = freeIndices.back();
			freeIndices.pop_back();
			_outCpuDescriptorHandle->ptr = heapStartCpu.ptr + (idx * heapHandleIncrement);
			_outGpuDescriptorHandle->ptr = heapStartGpu.ptr + (idx * heapHandleIncrement);
		}
		void Free(D3D12_CPU_DESCRIPTOR_HANDLE _outCpuDescriptorHandle, D3D12_GPU_DESCRIPTOR_HANDLE _outGpuDescriptorHandle)
		{
			int cpuIndex = (int)((_outCpuDescriptorHandle.ptr - heapStartCpu.ptr) / heapHandleIncrement);
			int gpuIndex= (int)((_outGpuDescriptorHandle.ptr - heapStartGpu.ptr) / heapHandleIncrement);
			STEINS_CORE_ASSERT(cpuIndex == gpuIndex, "");
			freeIndices.push_back(cpuIndex);
		}
	};

	class D3D12GraphicsDevice : public GraphicsDevice
	{
	public:
		D3D12GraphicsDevice();
		virtual ~D3D12GraphicsDevice();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;

		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12CommandQueue* GetCommandQueue() const { return commandQueue.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
		ID3D12DescriptorHeap* GetSRVHeap() const { return srvHeap.Get(); }
		DescriptorHeapAllocator& GetSRVHeapAlloc() { return srvHeapAlloc; }
		IDXGIFactory7* GetFactory() const { return dxgiFactory.Get(); }

		//void WaitForGPU(IDXGISwapChain3* _swapChain);

	private:
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<ID3D12CommandAllocator> commandAllocator[2];

		// ComPtr<ID3D12Resource> renderTargets;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12DescriptorHeap> rtvHeap;
		ComPtr<ID3D12DescriptorHeap> srvHeap;
		ComPtr<ID3D12PipelineState> pipelineState;
		ComPtr<ID3D12GraphicsCommandList> commandList;

		DescriptorHeapAllocator srvHeapAlloc;

		ComPtr<IDXGIFactory7> dxgiFactory;
		ComPtr<IDXGIAdapter4> dxgiAdapter;
		ComPtr<ID3D12Debug> debugLayer;

		

	};
}

