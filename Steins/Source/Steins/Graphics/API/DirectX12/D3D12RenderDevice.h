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
		Array<int>            freeIndices;

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

		void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* _outCpuDescriptorHandle)
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

		virtual Shared<RenderContext> CreateContext() override;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _bufferSize, UInt32 _stride) override;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<RenderPass> CreateRenderPass(const RenderPassDesc& _desc) override;
		virtual Shared<Framebuffer> CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc & _desc) override;
		virtual Shared<PipelineState> CreatePipelineState(const PipelineStateDesc& _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<Swapchain> CreateSwapchain(SteinsWindow* _window, const SwapchainDesc& _desc)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path, const TextureDesc& _desc)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) override;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) override;

		ID3D12Device* GetDevice() const { return device.Get(); }
		ID3D12CommandQueue* GetCommandQueue() const { return commandQueue.Get(); }
		void SetCommandList(ID3D12GraphicsCommandList* _commandList) { commandList = _commandList; }
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

		ComPtr<ID3D12GraphicsCommandList> BeginSingleTimeCommands();
		void EndSingleTimeCommands(ComPtr<ID3D12GraphicsCommandList> _commandBuffer);

		//void WaitForGPU(IDXGISwapChain3* _swapChain);

	private:
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12CommandQueue> commandQueue;

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
		ComPtr<ID3D12Debug5> debugLayer;



	};
}

