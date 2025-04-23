#include "SteinsPCH.h"
#include "D3D12GraphicsDevice.h"
#include "D3D12RendererAPI.h"
#include "D3D12Framebuffer.h"
#include "D3D12PipelineState.h"
#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	D3D12GraphicsDevice::D3D12GraphicsDevice()
	{
		API = RendererAPIType::DirectX12;
	}

	D3D12GraphicsDevice::~D3D12GraphicsDevice()
	{
#if defined(DEBUG) || defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
		if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
		{
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE); // 원하면 TRUE로

			// 필요 없는 메시지는 필터링도 가능
			D3D12_MESSAGE_ID denyIds[] = {
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
			};
			D3D12_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;
			infoQueue->AddStorageFilterEntries(&filter);
		}
#endif
	}

	void D3D12GraphicsDevice::Init()
	{
#if defined(_DEBUG)
		// Enable the D3D12 debug layer.
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
		{
			debugLayer->EnableDebugLayer();
		}
#endif
		// DXGI 팩토리 생성
		HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
		if (FAILED(hr))
		{
			STEINS_CORE_ERROR("Failed to create dxgiFactory!");
		}
		
		// 디바이스 생성
		int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0
		bool isAdapterFound = false; // set this to true when a good one was found
		for (UINT adapterIndex = 0;
			dxgiFactory->EnumAdapterByGpuPreference(
				adapterIndex,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(&dxgiAdapter)
			) != DXGI_ERROR_NOT_FOUND;
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC3 desc;
			dxgiAdapter->GetDesc3(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
			{
				isAdapterFound = true;
				break;
			}
		}

		STEINS_CORE_ASSERT(isAdapterFound, "Failed to find Adapter!");

		hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.GetAddressOf()));
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to Create Device!");

		// 커맨드 큐 생성
		{
			D3D12_COMMAND_QUEUE_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue));

			for (int i = 0; i < 2; i++)
			{
				HRESULT hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator[i].GetAddressOf()));
				STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create command allocator {0}", i);
			}
			

			device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[0].Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()));

			//commandList->Close();
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.NumDescriptors = 64;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			desc.NodeMask = 0;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(srvHeap.GetAddressOf()));
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create SRV descriptor heap");
			srvHeapAlloc.Create(device.Get(), srvHeap.Get());

			commandList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
		}
	

		DXGI_ADAPTER_DESC3 adapterDescription; // Vendor
		SecureZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));
		char videoCardDescription[128]; // Renderer
		LARGE_INTEGER driverVersion; // Version

		dxgiAdapter->GetDesc3(&adapterDescription);
		dxgiAdapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

		// Renderer
		wcstombs_s(NULL, videoCardDescription, 128, adapterDescription.Description, 128);

		// Version
		std::string major, minor, release, build;
		major = std::to_string(HIWORD(driverVersion.HighPart));
		minor = std::to_string(LOWORD(driverVersion.HighPart));
		release = std::to_string(HIWORD(driverVersion.LowPart));
		build = std::to_string(LOWORD(driverVersion.LowPart));

		std::string version;
		version = major + "." + minor + "." + release + "." + build;

		STEINS_CORE_INFO("DirectX11 Info:");
		STEINS_CORE_INFO("  Vendor: {0}", GraphicsUtil::GetVendor(adapterDescription.VendorId));
		STEINS_CORE_INFO("  Renderer: {0} {1} GB", videoCardDescription, std::round((double)adapterDescription.DedicatedVideoMemory/ (1<<30)));
		STEINS_CORE_INFO("  Version: {0}", version);

		//device->CreateDescriptorHeap()
		//// 스왑 체인 생성
		//DXGI_SWAP_CHAIN_DESC swapDesc = {};
		//swapDesc.BufferCount = 2;
		//swapDesc.BufferDesc.Width = 800;
		//swapDesc.BufferDesc.Height = 600;
		//swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//swapDesc.OutputWindow = hwnd;
		//swapDesc.Windowed = TRUE;
		//swapDesc.SampleDesc.Count = 1;
		//dxgiFactory->CreateSwapChain(commandQueue.Get(), &swapDesc, &swapChain);

	}

	void D3D12GraphicsDevice::Shutdown()
	{
	}

	void D3D12GraphicsDevice::Render()
	{
	}

	Shared<VertexBuffer> D3D12GraphicsDevice::CreateVertexBuffer(Float32* _vertices, UInt32 _size)
	{
		return Shared<VertexBuffer>();
	}

	Shared<IndexBuffer> D3D12GraphicsDevice::CreateIndexBuffer(UInt32* _indices, UInt32 _count)
	{
		return Shared<IndexBuffer>();
	}

	Shared<Framebuffer> D3D12GraphicsDevice::CreateFramebuffer(FramebufferSpecification _spec)
	{
		return Shared<Framebuffer>();
	}

	Shared<PipelineState> D3D12GraphicsDevice::CreatePipelineState(PipelineStateDesc _desc)
	{
		return Shared<PipelineState>();
	}

	Shared<Shader> D3D12GraphicsDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return Shared<Shader>();
	}

	Shared<SwapChain> D3D12GraphicsDevice::CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)
	{
		return Shared<SwapChain>();
	}

	Shared<Texture2D> D3D12GraphicsDevice::CreateTexture2D(const FilePath& _path)
	{
		return Shared<Texture2D>();
	}

	Unique<ImGuiRenderer> D3D12GraphicsDevice::CreateImGuiRenderer()
	{
		return Unique<ImGuiRenderer>();
	}

	//void D3D12GraphicsDevice::WaitForGPU(IDXGISwapChain3* _swapChain)
	//{
	//	HRESULT hr;

	//	// swap the current rtv buffer index so we draw on the correct buffer
	//	int frameIndex = _swapChain->GetCurrentBackBufferIndex();

	//	// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
	//	// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
	//	if (fence[frameIndex]->GetCompletedValue() < fenceValue[frameIndex])
	//	{
	//		// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
	//		hr = fence[frameIndex]->SetEventOnCompletion(fenceValue[frameIndex], fenceEvent);
	//		// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
	//		// has reached "fenceValue", we know the command queue has finished executing
	//		WaitForSingleObject(fenceEvent, INFINITE);
	//	}

	//	// increment fenceValue for next frame
	//	fenceValue[frameIndex]++;
	//}



}


