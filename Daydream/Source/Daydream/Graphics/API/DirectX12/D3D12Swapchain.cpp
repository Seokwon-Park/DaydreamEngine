#include "DaydreamPCH.h"
#include "D3D12Swapchain.h"

#include "D3D12Utility.h"

#include "Daydream/Graphics/Core/Renderer2D.h"

#include "Daydream/Core/Window.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Daydream
{
	D3D12Swapchain::D3D12Swapchain(D3D12RenderDevice* _device, const DaydreamWindow& _window, const SwapchainDesc& _desc)
	{
		device = _device;
		desc = _desc;
		bufferCount = _desc.bufferCount;
		format = GraphicsUtility::DirectX::ConvertToDXGIFormat(_desc.format);;
		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;    // 샘플 수 (1이면 MSAA 비활성화)
		sampleDesc.Quality = 0;  // 품질 레벨 (0이면 기본값)

		DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
		ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
		swapchainDesc.Width = _desc.width;
		swapchainDesc.Height = _desc.height;
		swapchainDesc.Format = format;
		swapchainDesc.Stereo = 0;
		swapchainDesc.SampleDesc = sampleDesc;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.BufferCount = _desc.bufferCount;
		//swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

		windowHandle = glfwGetWin32Window((GLFWwindow*)_window.GetNativeWindow());

		ComPtr<IDXGISwapChain1> swapChain1;
		HRESULT hr = device->GetFactory()->CreateSwapChainForHwnd(
			device->GetCommandQueue(),
			windowHandle,
			&swapchainDesc,
			nullptr, nullptr,
			swapChain1.GetAddressOf()
		);

		swapChain1->QueryInterface(swapchain.GetAddressOf());
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create swapChain!");

		commandLists.resize(_desc.bufferCount);
		for (UInt32 i = 0; i < _desc.bufferCount; i++)
		{
			// 매 루프마다 '진짜로' 새로운 커맨드 리스트와 Allocator를 각각 생성합니다!
			commandLists[i] = MakeShared<D3D12RenderCommandList>(device);
		}

		RenderPassAttachmentDesc colorDesc;
		colorDesc.format = _desc.format;
		colorDesc.isSwapchain = true;

		RenderPassDesc rpDesc{};
		rpDesc.colorAttachments.push_back(colorDesc);

		mainRenderPass = MakeShared<D3D12RenderPass>(device, rpDesc);

		framebuffers.resize(desc.bufferCount);
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			ComPtr<ID3D12Resource> backBuffer;
			swapchain->GetBuffer(i, IID_PPV_ARGS(backBuffer.GetAddressOf()));
			framebuffers[i] = MakeShared<D3D12Framebuffer>(device, mainRenderPass.get(), this, backBuffer.Get());
			d3d12Backbuffers.push_back(backBuffer);
		}
		frameIndex = swapchain->GetCurrentBackBufferIndex();

		fenceValues.resize(_desc.bufferCount);
		//fence가 0까지는 완료된 상태로 생성
		hr = device->GetDevice()->CreateFence(fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
		//fenceValues[frameIndex]++;
		//fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		fenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
		if (fenceEvent == nullptr)
		{
			DAYDREAM_CORE_ERROR("Failed to Create FenceEvent!");
		}

		currentFenceValue = 0;
	}
	D3D12Swapchain::~D3D12Swapchain()
	{
		WaitForGPU();
		device = nullptr;
	}
	void D3D12Swapchain::SetVSync(bool _enabled)
	{
	}

	void D3D12Swapchain::Present()
	{
		swapchain->Present(desc.isVSync, 0);
		//frameIndex = 0->1
		frameIndex = swapchain->GetCurrentBackBufferIndex();
	}


	void D3D12Swapchain::BeginFrame()
	{
		if (isSwapchainResized)
		{
			WaitForGPU();
			//이제 currentFenceValue까지 작업이 다 끝난 상태

			d3d12Backbuffers.clear();
			framebuffers.clear();

			swapchain->ResizeBuffers(desc.bufferCount, desc.width, desc.height, format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);

			framebuffers.resize(desc.bufferCount);
			for (UInt32 i = 0; i < desc.bufferCount; i++)
			{
				ComPtr<ID3D12Resource> backBuffer;
				swapchain->GetBuffer(i, IID_PPV_ARGS(backBuffer.GetAddressOf()));
				framebuffers[i] = MakeShared<D3D12Framebuffer>(device, mainRenderPass.get(), this, backBuffer.Get());
				d3d12Backbuffers.push_back(backBuffer);
			}

			frameIndex = swapchain->GetCurrentBackBufferIndex();
			for (UInt32 i = 0; i < desc.bufferCount; i++)
			{
				fenceValues[i] = currentFenceValue;
			}

			isSwapchainResized = false;
		}
		
		// frameIndex의 작업이 끝난상태인지 확인
		if (fence->GetCompletedValue() < fenceValues[frameIndex])
		{
			fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent.Get());
			WaitForSingleObjectEx(fenceEvent.Get(), INFINITE, FALSE);
		}


		currentCommandList = commandLists[frameIndex]->GetID3D12GraphicsCommandList();

		commandLists[frameIndex]->Begin();

		ID3D12DescriptorHeap* heaps[] = { device->GetCBVSRVUAVHeap(), device->GetSamplerHeap() };
		currentCommandList->SetDescriptorHeaps(2, heaps);

		D3D12_RESOURCE_BARRIER barr{};
		barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barr.Transition.pResource = d3d12Backbuffers[frameIndex].Get();
		barr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		currentCommandList->ResourceBarrier(1, &barr);
	}

	void D3D12Swapchain::EndFrame()
	{
		D3D12_RESOURCE_BARRIER barr{};

		barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barr.Transition.pResource = d3d12Backbuffers[frameIndex].Get();
		barr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		currentCommandList->ResourceBarrier(1, &barr);

		commandLists[frameIndex]->End();

		Array<ID3D12CommandList*> execCommandLists = { currentCommandList };
		//이제 0번 이미지에 그려
		device->GetCommandQueue()->ExecuteCommandLists((UInt32)execCommandLists.size(), execCommandLists.data());

		//여기서 FenceValue = 0 -> 1
		currentFenceValue++;
		//다그리면 Fence에 1 전달해
		device->GetCommandQueue()->Signal(fence.Get(), currentFenceValue);

		//FenceValue[0] = 1; // 이제 0번 이미지 다시 쓰려면 Fence가 1보다 커야된다
		fenceValues[frameIndex] = currentFenceValue;
	}

	//모든 GPU작업이 끝날때까지 대기
	void D3D12Swapchain::WaitForGPU()
	{
		currentFenceValue++;
		//currentFenceValue 시그널해
		device->GetCommandQueue()->Signal(fence.Get(), currentFenceValue);

		//currentFenceValue 받을때까지 기다려
		fence->SetEventOnCompletion(currentFenceValue, fenceEvent.Get());
		WaitForSingleObjectEx(fenceEvent.Get(), INFINITE, FALSE);
	}

	// GPU가 이전 프레임 작업을 끝낼 때까지 기다림
	void D3D12Swapchain::MoveToNextFrame()
	{
		// 1. 현재 프레임의 커맨드 제출 완료를 위한 펜스 값 기록 요청
		const UINT64 currentFenceValue = fenceValues[frameIndex]; // 현재 프레임의 '고유한' 펜스 값
		HRESULT hr = device->GetCommandQueue()->Signal(fence.Get(), currentFenceValue);
		// fence값 1로 만들어
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to signal!");

		// 2. 다음 백 버퍼 인덱스 획득 (GPU가 렌더링을 마친 버퍼를 가져옴) Present이후 호출이므로 바뀜
		frameIndex = swapchain->GetCurrentBackBufferIndex();

		// 3. 다음 프레임에 사용될 백 버퍼의 펜스 값 확인 및 대기
		 //    GPU가 이전에 (이 'frameIndex'에 해당하는 백 버퍼를 사용했던) 작업을 완료했는지 확인
		if (fence->GetCompletedValue() < fenceValues[frameIndex]) // 현재 프레임 인덱스(새로 갱신된)의 펜스 값과 비교
		{
			// 완료되지 않았다면, 해당 펜스 값이 시그널될 때까지 CPU 대기
			fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent.Get());
			WaitForSingleObjectEx(fenceEvent.Get(), INFINITE, FALSE);
		}
		fenceValues[frameIndex] = currentFenceValue + 1;
	}
}
