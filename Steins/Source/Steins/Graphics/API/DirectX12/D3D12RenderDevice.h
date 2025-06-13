#pragma once

#include "Steins/Graphics/Core/RenderDevice.h"

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
			if (heapType != D3D12_DESCRIPTOR_HEAP_TYPE_RTV &&
				heapType != D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
			{
				heapStartGpu = heap->GetGPUDescriptorHandleForHeapStart();
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

		void AllocCPU(D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuDescriptorHandle)
		{
			STEINS_CORE_ASSERT(freeIndices.size() > 0, "");
			int idx = freeIndices.back();
			freeIndices.pop_back();
			_outCpuDescriptorHandle->ptr = heapStartCpu.ptr + (idx * heapHandleIncrement);
		}

		void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuDescriptorHandle, D3D12_GPU_DESCRIPTOR_HANDLE* _outGpuDescriptorHandle)
		{
			STEINS_CORE_ASSERT(freeIndices.size() > 0, "");
			STEINS_CORE_ASSERT(heapType != D3D12_DESCRIPTOR_HEAP_TYPE_RTV, "");
			int idx = freeIndices.back();
			freeIndices.pop_back();
			_outCpuDescriptorHandle->ptr = heapStartCpu.ptr + (idx * heapHandleIncrement);
			_outGpuDescriptorHandle->ptr = heapStartGpu.ptr + (idx * heapHandleIncrement);
		}
		void Free(D3D12_CPU_DESCRIPTOR_HANDLE _outCpuDescriptorHandle, D3D12_GPU_DESCRIPTOR_HANDLE _outGpuDescriptorHandle)
		{
			int cpuIndex = (int)((_outCpuDescriptorHandle.ptr - heapStartCpu.ptr) / heapHandleIncrement);
			int gpuIndex = (int)((_outGpuDescriptorHandle.ptr - heapStartGpu.ptr) / heapHandleIncrement);
			STEINS_CORE_ASSERT(cpuIndex == gpuIndex, "");
			freeIndices.push_back(cpuIndex);
		}
	};

	class D3D12RenderDevice : public RenderDevice
	{
	public:
		D3D12RenderDevice();
		virtual ~D3D12RenderDevice();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<GraphicsContext> CreateContext() override;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _bufferSize, UInt32 _stride) override;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<Framebuffer> CreateFramebuffer(FramebufferDesc _spec)override;
		virtual Shared<PipelineState> CreatePipelineState(PipelineStateDesc _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) override;

		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12CommandQueue* GetCommandQueue() const { return commandQueue.Get(); }
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
		ID3D12CommandAllocator* GetCommandAllocator(UInt32 _index) const { return commandAllocators[_index].Get(); }
		ID3D12DescriptorHeap* GetSRVHeap() const { return cbvSrvUavHeap.Get(); }
		DescriptorHeapAllocator& GetRTVHeapAlloc() { return rtvHeapAlloc; }
		DescriptorHeapAllocator& GetCBVSRVUAVHeapAlloc() { return cbvSrvUavHeapAlloc; }
		IDXGIFactory7* GetFactory() const { return dxgiFactory.Get(); }

		ComPtr<ID3D12GraphicsCommandList> BeginSingleTimeCommands();
		void EndSingleTimeCommands(ComPtr<ID3D12GraphicsCommandList> _commandBuffer);

		//void WaitForGPU(IDXGISwapChain3* _swapChain);

	private:
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<ID3D12CommandAllocator> commandAllocators[2];

		// ComPtr<ID3D12Resource> renderTargets;
		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12DescriptorHeap> rtvHeap;
		ComPtr<ID3D12DescriptorHeap> cbvSrvUavHeap;
		ComPtr<ID3D12PipelineState> pipelineState;
		ComPtr<ID3D12GraphicsCommandList> commandList;

		DescriptorHeapAllocator rtvHeapAlloc;
		DescriptorHeapAllocator cbvSrvUavHeapAlloc;

		ComPtr<IDXGIFactory7> dxgiFactory;
		ComPtr<IDXGIAdapter4> dxgiAdapter;
		ComPtr<ID3D12Debug5> debugLayer;



	};
}

