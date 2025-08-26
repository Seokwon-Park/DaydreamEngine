#pragma once

#include "Daydream/Graphics/Core/RenderDevice.h"

namespace Daydream
{
	struct DescriptorHeapAllocator
	{
		ID3D12DescriptorHeap* heap = nullptr;
		D3D12_DESCRIPTOR_HEAP_TYPE  heapType = D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES;
		D3D12_CPU_DESCRIPTOR_HANDLE heapStartCpu;
		D3D12_GPU_DESCRIPTOR_HANDLE heapStartGpu;
		UINT                        heapHandleIncrement;
		Array<int>            freeIndices;

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
	};

	class D3D12RenderDevice : public RenderDevice
	{
	public:
		D3D12RenderDevice();
		virtual ~D3D12RenderDevice();

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<RenderContext> CreateContext() override;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize = 0, const void* _initialData = nullptr) override;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(const UInt32 * _indices, UInt32 _count) override;
		virtual Shared<RenderPass> CreateRenderPass(const RenderPassDesc& _desc) override;
		virtual Shared<Framebuffer> CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc) override;
		virtual Shared<PipelineState> CreatePipelineState(const PipelineStateDesc& _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<Swapchain> CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)override;
		virtual Shared<Texture2D> CreateTexture2D(const void* _imageData, const TextureDesc& _desc)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) override;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) override;

		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12CommandQueue* GetCommandQueue() const { return commandQueue.Get(); }
		void SetCommandList(ComPtr<ID3D12GraphicsCommandList> _commandList) { commandList = _commandList; }
		ID3D12CommandAllocator* GetCommandAlloc() const { return allocator.Get(); }
		void SetCommandAlloc(ComPtr<ID3D12CommandAllocator> _commandAlloc) { allocator = _commandAlloc; }
		ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
		ID3D12DescriptorHeap* GetRTVHeap() const { return rtvHeap.Get(); }
		ID3D12DescriptorHeap* GetDSVHeap() const { return dsvHeap.Get(); }
		ID3D12DescriptorHeap* GetSamplerHeap() const { return samplerHeap.Get(); }
		ID3D12DescriptorHeap* GetCBVSRVUAVHeap() const { return cbvSrvUavHeap.Get(); }
		DescriptorHeapAllocator& GetRTVHeapAlloc() { return rtvHeapAlloc; }
		DescriptorHeapAllocator& GetDSVHeapAlloc() { return dsvHeapAlloc; }
		DescriptorHeapAllocator& GetSamplerHeapAlloc() { return samplerHeapAlloc; }
		DescriptorHeapAllocator& GetCBVSRVUAVHeapAlloc() { return cbvSrvUavHeapAlloc; }
		IDXGIFactory7* GetFactory() const { return dxgiFactory.Get(); }

		void ExecuteSingleTimeCommands(std::function<void(ID3D12GraphicsCommandList*)> commands);
		ComPtr<ID3D12Resource> CreateBuffer(UINT64 _size,
			D3D12_HEAP_TYPE _heapType,
			D3D12_RESOURCE_STATES _initialState,
			D3D12_RESOURCE_FLAGS _flags = D3D12_RESOURCE_FLAG_NONE);
		void CopyBuffer(ID3D12Resource* _src, ID3D12Resource* _dst, UInt32 _dataSize);

		ComPtr<ID3D12Resource> CreateTexture2D(
			UINT _width, 
			UINT _height,
			DXGI_FORMAT _format,
			D3D12_RESOURCE_FLAGS _flags, // vk::ImageUsageFlags¿¡ ÇØ´ç
			D3D12_RESOURCE_STATES _initialState
		);
		void CopyBufferToImage(ID3D12Resource* _src, ID3D12Resource* _dst);

		void TransitionResourceState(
			ID3D12Resource* _resource,
			D3D12_RESOURCE_STATES _stateBefore,
			D3D12_RESOURCE_STATES _stateAfter);


		//void WaitForGPU(IDXGISwapChain3* _swapChain);

	private:
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;
		ComPtr<ID3D12CommandAllocator> allocator;



		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12DescriptorHeap> rtvHeap;
		ComPtr<ID3D12DescriptorHeap> dsvHeap;
		ComPtr<ID3D12DescriptorHeap> samplerHeap;
		ComPtr<ID3D12DescriptorHeap> cbvSrvUavHeap;
		ComPtr<ID3D12PipelineState> pipelineState;
		ComPtr<ID3D12GraphicsCommandList> commandList;

		DescriptorHeapAllocator rtvHeapAlloc;
		DescriptorHeapAllocator dsvHeapAlloc;
		DescriptorHeapAllocator samplerHeapAlloc;
		DescriptorHeapAllocator cbvSrvUavHeapAlloc;

		ComPtr<IDXGIFactory7> dxgiFactory;
		ComPtr<IDXGIAdapter4> dxgiAdapter;
		ComPtr<ID3D12Debug> debugLayer;

		ComPtr<ID3D12CommandAllocator> uploadCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> uploadCommandList;
		ComPtr<ID3D12Fence> uploadFence;
		UInt64 uploadFenceValue;
		HANDLE uploadFenceEvent;


	};
}

