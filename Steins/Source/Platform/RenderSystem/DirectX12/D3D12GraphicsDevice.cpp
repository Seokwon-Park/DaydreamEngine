#include "SteinsPCH.h"
#include "D3D12GraphicsDevice.h"

namespace Steins
{
	D3D12GraphicsDevice::D3D12GraphicsDevice()
	{
	}

	D3D12GraphicsDevice::~D3D12GraphicsDevice()
	{
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

		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceEvent == nullptr)
		{
			STEINS_CORE_ERROR("WTF? fence Event");
		}
		
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

	void D3D12GraphicsDevice::SwapBuffers()
	{
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

	void D3D12GraphicsDevice::SignalFence(ID3D12Fence* _fence, UINT _value)
	{
		commandQueue->Signal(_fence, _value);
	}


}


