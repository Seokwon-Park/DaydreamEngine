#pragma once

namespace Daydream
{
	class DescriptorHeapAllocator
	{
	public:
		void Create(ID3D12Device* _device, ID3D12DescriptorHeap* _heap)
		{
			DAYDREAM_CORE_ASSERT(heap == nullptr && freeIndices.empty(), "Can't Create Heap Allocator");
			heap = _heap;
			D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();
			heapType = desc.Type;
			heapStartCpu = heap->GetCPUDescriptorHandleForHeapStart();
			if (heapType != D3D12_DESCRIPTOR_HEAP_TYPE_RTV &&
				heapType != D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
			{
				heapStartGpu = heap->GetGPUDescriptorHandleForHeapStart();
			}
			else
			{
				heapStartGpu.ptr = 0;
			}
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

		void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuDescriptorHandle)
		{
			DAYDREAM_CORE_ASSERT(freeIndices.size() > 0, "");
			int idx = freeIndices.back();
			freeIndices.pop_back();
			_outCpuDescriptorHandle->ptr = heapStartCpu.ptr + (idx * heapHandleIncrement);
		}

		void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuDescriptorHandle, D3D12_GPU_DESCRIPTOR_HANDLE* _outGpuDescriptorHandle)
		{
			DAYDREAM_CORE_ASSERT(freeIndices.size() > 0, "");
			DAYDREAM_CORE_ASSERT(heapType != D3D12_DESCRIPTOR_HEAP_TYPE_RTV, "");
			int idx = freeIndices.back();
			freeIndices.pop_back();
			_outCpuDescriptorHandle->ptr = heapStartCpu.ptr + (idx * heapHandleIncrement);
			_outGpuDescriptorHandle->ptr = heapStartGpu.ptr + (idx * heapHandleIncrement);
		}
		void Free(D3D12_CPU_DESCRIPTOR_HANDLE _outCpuDescriptorHandle)
		{
			int cpuIndex = (int)((_outCpuDescriptorHandle.ptr - heapStartCpu.ptr) / heapHandleIncrement);
			freeIndices.push_back(cpuIndex);
		}
		void Free(D3D12_CPU_DESCRIPTOR_HANDLE _outCpuDescriptorHandle, D3D12_GPU_DESCRIPTOR_HANDLE _outGpuDescriptorHandle)
		{
			int cpuIndex = (int)((_outCpuDescriptorHandle.ptr - heapStartCpu.ptr) / heapHandleIncrement);
			int gpuIndex = (int)((_outGpuDescriptorHandle.ptr - heapStartGpu.ptr) / heapHandleIncrement);
			DAYDREAM_CORE_ASSERT(cpuIndex == gpuIndex, "");
			freeIndices.push_back(cpuIndex);
		}
	private:
		ID3D12DescriptorHeap*		heap = nullptr;
		D3D12_DESCRIPTOR_HEAP_TYPE  heapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		D3D12_CPU_DESCRIPTOR_HANDLE heapStartCpu;
		D3D12_GPU_DESCRIPTOR_HANDLE heapStartGpu;
		UINT                        heapHandleIncrement;
		Array<UInt32>					freeIndices;
	};


	struct DynamicDescriptorHeapAllocator
	{
		// 반환값을 담을 간단한 구조체
		struct Allocation 
		{
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		};

		ID3D12DescriptorHeap*		heap = nullptr;
		UINT                        heapHandleIncrement = 0;
		UINT                        numDescriptors = 0;

		UINT                          currentOffset = 0;

		D3D12_CPU_DESCRIPTOR_HANDLE   heapStartCpu;
		D3D12_GPU_DESCRIPTOR_HANDLE   heapStartGpu;

		void Create(ID3D12Device* _device, ID3D12DescriptorHeap* _heap)
		{
			heap = _heap;
			D3D12_DESCRIPTOR_HEAP_DESC desc = heap->GetDesc();

			// 이 힙은 Shader Visible이어야 합니다.
			DAYDREAM_CORE_ASSERT(desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, "Dynamic heap must be shader visible!");

			heapHandleIncrement = _device->GetDescriptorHandleIncrementSize(desc.Type);
			numDescriptors = desc.NumDescriptors;

			heapStartCpu = heap->GetCPUDescriptorHandleForHeapStart();
			heapStartGpu = heap->GetGPUDescriptorHandleForHeapStart();
			currentOffset = 0;
		}

		void Destroy()
		{
			heap = nullptr;
		}

		// --- 새로 추가된 메서드 ---
		// 매 프레임 시작 시 호출하여 리셋
		void BeginFrame()
		{
			currentOffset = 0;
		}

		// --- 완전히 변경된 Alloc 메서드 ---
		// _count만큼의 연속된 디스크립터 블록을 할당합니다.
		Allocation Alloc(UINT _count = 1)
		{
			// 남은 공간이 충분한지 확인
			DAYDREAM_CORE_ASSERT(currentOffset + _count <= numDescriptors, "Dynamic heap is full!");

			// 현재 위치를 기준으로 핸들 계산
			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; 
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;

			cpuHandle.ptr = heapStartCpu.ptr + currentOffset * heapHandleIncrement;
			gpuHandle.ptr = heapStartGpu.ptr + currentOffset * heapHandleIncrement;

			// 다음 할당을 위해 오프셋을 이동
			currentOffset += _count;

			// 할당된 블록의 시작 핸들들을 반환
			return { cpuHandle, gpuHandle };
		}
	};
}