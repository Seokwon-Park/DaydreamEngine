#include "SteinsPCH.h"
#include "D3D12SwapChain.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

#include "Steins/Core/Window.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Steins
{
	D3D12SwapChain::D3D12SwapChain(GraphicsDevice* _device, SwapChainDesc* _desc, SteinsWindow* _window)
	{
		device = Cast<D3D12GraphicsDevice>(_device);
		desc = *_desc;
		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;    // 샘플 수 (1이면 MSAA 비활성화)
		sampleDesc.Quality = 0;  // 품질 레벨 (0이면 기본값)

		DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
		ZeroMemory(&swapchainDesc, sizeof(swapchainDesc));
		swapchainDesc.Width = _desc->width;
		swapchainDesc.Height = _desc->height;
		swapchainDesc.Format = GraphicsUtil::RenderFormatToDXGIFormat(_desc->format);
		swapchainDesc.Stereo = 0;
		swapchainDesc.SampleDesc = sampleDesc;
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.BufferCount = _desc->bufferCount;
		swapchainDesc.Scaling = DXGI_SCALING_NONE;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapchainDesc.Flags = 0;

		ComPtr<IDXGISwapChain1> swapChain1;
		HRESULT hr = device->GetFactory()->CreateSwapChainForHwnd(
			device->GetCommandQueue(),
			glfwGetWin32Window((GLFWwindow*)_window->GetNativeWindow()),
			&swapchainDesc,
			nullptr, nullptr,
			swapChain1.GetAddressOf()
		);

		hr = device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
		for (UINT i = 0; i < swapchainDesc.BufferCount; i++)
		{
			fenceValues[i] = 0; // set the initial fence value to 0
		}

		swapChain1->QueryInterface(swapChain.GetAddressOf());
		frameIndex = swapChain->GetCurrentBackBufferIndex();

		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceEvent == nullptr)
		{
			STEINS_CORE_ERROR("Failed to Create FenceEvent!");
		}

		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create swapChain!");



	}
	D3D12SwapChain::~D3D12SwapChain()
	{
		WaitForGPU();
		device = nullptr;
	}
	void D3D12SwapChain::SetVSync(bool _enabled)
	{
	}

	void D3D12SwapChain::SwapBuffers()
	{
		swapChain->Present(desc.isVSync, 0);

		MoveToNextFrame();
	}
	void D3D12SwapChain::WaitForGPU()
	{
		// Schedule a Signal command in the queue.
		device->GetCommandQueue()->Signal(fence.Get(), fenceValues[frameIndex]);

		// Wait until the fence has been processed.
		fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent);
		WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);

		// Increment the fence value for the current frame.
		fenceValues[frameIndex]++;
	}

	// GPU가 이전 프레임 작업을 끝낼 때까지 기다림
	void D3D12SwapChain::MoveToNextFrame()
	{
		const UINT64 currentFenceValue = fenceValues[frameIndex];
		device->GetCommandQueue()->Signal(fence.Get(), fenceValues[frameIndex]);

		frameIndex = swapChain->GetCurrentBackBufferIndex();

		if (fence->GetCompletedValue() < fenceValues[frameIndex])
		{
			fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent);
			WaitForSingleObject(fenceEvent, INFINITE);
		}

		fenceValues[frameIndex] = currentFenceValue + 1;
	}
}
