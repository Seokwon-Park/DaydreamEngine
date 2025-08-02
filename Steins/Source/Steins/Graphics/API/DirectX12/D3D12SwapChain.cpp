#include "SteinsPCH.h"
#include "D3D12Swapchain.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"

#include "Steins/Graphics/Core/Renderer2D.h"

#include "Steins/Core/Window.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Steins
{
	D3D12Swapchain::D3D12Swapchain(RenderDevice* _device, SteinsWindow* _window, const SwapchainDesc& _desc)
	{
		device = Cast<D3D12RenderDevice>(_device);
		desc = _desc;
		bufferCount = _desc.bufferCount;
		format = GraphicsUtil::ConvertRenderFormatToDXGIFormat(_desc.format);;
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

		windowHandle = glfwGetWin32Window((GLFWwindow*)_window->GetNativeWindow());

		ComPtr<IDXGISwapChain1> swapChain1;
		HRESULT hr = device->GetFactory()->CreateSwapChainForHwnd(
			device->GetCommandQueue(),
			windowHandle,
			&swapchainDesc,
			nullptr, nullptr,
			swapChain1.GetAddressOf()
		);

		swapChain1->QueryInterface(swapChain.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create swapChain!");

		commandLists.resize(_desc.bufferCount);
		commandAllocators.resize(_desc.bufferCount);
		for (UInt32 i = 0; i < _desc.bufferCount; i++)
		{
			HRESULT hr = device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocators[i].GetAddressOf()));
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create command allocator {0}", i);

			hr = device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[i].Get(), nullptr, IID_PPV_ARGS(commandLists[i].GetAddressOf()));
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create commandlist");

			commandLists[i]->Close();
		}

		RenderPassAttachmentDesc colorDesc;
		colorDesc.format = _desc.format;

		RenderPassDesc rpDesc{};
		rpDesc.colorAttachments.push_back(colorDesc);
		rpDesc.isSwapchain = true;

		mainRenderPass = MakeShared<D3D12RenderPass>(device, rpDesc);

		framebuffers.resize(desc.bufferCount);
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			frameIndex = i;
			framebuffers[i] = MakeShared<D3D12Framebuffer>(device, mainRenderPass.get(), this);
		}
		frameIndex = swapChain->GetCurrentBackBufferIndex();

		fenceValues.resize(_desc.bufferCount);
		//fence가 0까지는 완료된 상태로 생성
		hr = device->GetDevice()->CreateFence(fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
		//다음 펜스값은 1을 보내야함
		fenceValues[frameIndex]++;
		//fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		fenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
		if (fenceEvent == nullptr)
		{
			STEINS_CORE_ERROR("Failed to Create FenceEvent!");
		}

		BeginFrame();
	}
	D3D12Swapchain::~D3D12Swapchain()
	{
		WaitForGPU();
		device = nullptr;
	}
	void D3D12Swapchain::SetVSync(bool _enabled)
	{
	}

	void D3D12Swapchain::SwapBuffers()
	{
		EndFrame();

		MoveToNextFrame();

		BeginFrame();
	}
	void D3D12Swapchain::ResizeSwapchain(UInt32 _width, UInt32 _height)
	{
		EndFrame();

		WaitForGPU();

		framebuffers.clear();
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			fenceValues[i] = fenceValues[frameIndex];
		}

		swapChain->ResizeBuffers(desc.bufferCount, _width, _height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING);

		framebuffers.resize(desc.bufferCount);
		for (UInt32 i = 0; i < desc.bufferCount; i++)
		{
			frameIndex = i;
			framebuffers[i] = MakeShared<D3D12Framebuffer>(device, mainRenderPass.get(), this);
			//framebuffers[i]->Resize(_width, _height);
		}
		frameIndex = swapChain->GetCurrentBackBufferIndex();
		fenceValues[frameIndex]++;

		BeginFrame();
	}

	void D3D12Swapchain::BeginFrame()
	{
		device->SetCommandAlloc(commandAllocators[frameIndex]);
		commandAllocators[frameIndex]->Reset();
		device->SetCommandList(commandLists[frameIndex]);
		device->GetCommandList()->Reset(commandAllocators[frameIndex].Get(), nullptr);

		ID3D12DescriptorHeap* heaps[] = { device->GetCBVSRVUAVHeap(), device->GetSamplerHeap() };
		device->GetCommandList()->SetDescriptorHeaps(2, heaps);

		D3D12_RESOURCE_BARRIER barr{};

		barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barr.Transition.pResource = (ID3D12Resource*)framebuffers[frameIndex]->GetColorAttachmentTexture(0)->GetNativeHandle();
		barr.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barr.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		device->GetCommandList()->ResourceBarrier(1, &barr);
	}

	void D3D12Swapchain::EndFrame()
	{
		D3D12_RESOURCE_BARRIER barr{};

		barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barr.Transition.pResource = (ID3D12Resource*)framebuffers[frameIndex]->GetColorAttachmentTexture(0)->GetNativeHandle();
		barr.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barr.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		barr.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		device->GetCommandList()->ResourceBarrier(1, &barr);

		device->GetCommandList()->Close();

		Array<ID3D12CommandList*> execCommandLists = { commandLists[frameIndex].Get() };
		device->GetCommandQueue()->ExecuteCommandLists((UInt32)execCommandLists.size(), execCommandLists.data());
		
		swapChain->Present(desc.isVSync, 0);
	}

	//모든 GPU작업이 끝날때까지 대기
	void D3D12Swapchain::WaitForGPU()
	{
		// 1. 현재 큐에 시그널 명령 추가. GPU에게 현재 펜스 값(fenceValues[frameIndex])을 시그널하라고 요청
		device->GetCommandQueue()->Signal(fence.Get(), fenceValues[frameIndex]);

		// 2. CPU가 이벤트 대기: GPU가 위에서 요청한 펜스 값을 시그널할 때까지 기다림
		//    이벤트가 시그널되면 WaitForSingleObjectEx가 반환됩니다.
		fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent.Get());
		WaitForSingleObjectEx(fenceEvent.Get(), INFINITE, FALSE);

		// 3. 펜스 값 증가: 다음 번 Signal을 위해 현재 펜스 값을 1 증가시킴.
		//    이 펜스 값은 이후 commandQueue->Signal에서 사용될 예정.
		fenceValues[frameIndex]++;
	}

	// GPU가 이전 프레임 작업을 끝낼 때까지 기다림
	void D3D12Swapchain::MoveToNextFrame()
	{
		// 1. 현재 프레임의 커맨드 제출 완료를 위한 펜스 값 기록 요청
		const UINT64 currentFenceValue = fenceValues[frameIndex]; // 현재 프레임의 '고유한' 펜스 값
		HRESULT hr = device->GetCommandQueue()->Signal(fence.Get(), currentFenceValue);
		// fence값 1로 만들어
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to signal!");

		// 2. 다음 백 버퍼 인덱스 획득 (GPU가 렌더링을 마친 버퍼를 가져옴) Present이후 호출이므로 바뀜
		frameIndex = swapChain->GetCurrentBackBufferIndex();

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
