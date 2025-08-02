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
		sampleDesc.Count = 1;    // ���� �� (1�̸� MSAA ��Ȱ��ȭ)
		sampleDesc.Quality = 0;  // ǰ�� ���� (0�̸� �⺻��)

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
		//fence�� 0������ �Ϸ�� ���·� ����
		hr = device->GetDevice()->CreateFence(fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
		//���� �潺���� 1�� ��������
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

	//��� GPU�۾��� ���������� ���
	void D3D12Swapchain::WaitForGPU()
	{
		// 1. ���� ť�� �ñ׳� ��� �߰�. GPU���� ���� �潺 ��(fenceValues[frameIndex])�� �ñ׳��϶�� ��û
		device->GetCommandQueue()->Signal(fence.Get(), fenceValues[frameIndex]);

		// 2. CPU�� �̺�Ʈ ���: GPU�� ������ ��û�� �潺 ���� �ñ׳��� ������ ��ٸ�
		//    �̺�Ʈ�� �ñ׳εǸ� WaitForSingleObjectEx�� ��ȯ�˴ϴ�.
		fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent.Get());
		WaitForSingleObjectEx(fenceEvent.Get(), INFINITE, FALSE);

		// 3. �潺 �� ����: ���� �� Signal�� ���� ���� �潺 ���� 1 ������Ŵ.
		//    �� �潺 ���� ���� commandQueue->Signal���� ���� ����.
		fenceValues[frameIndex]++;
	}

	// GPU�� ���� ������ �۾��� ���� ������ ��ٸ�
	void D3D12Swapchain::MoveToNextFrame()
	{
		// 1. ���� �������� Ŀ�ǵ� ���� �ϷḦ ���� �潺 �� ��� ��û
		const UINT64 currentFenceValue = fenceValues[frameIndex]; // ���� �������� '������' �潺 ��
		HRESULT hr = device->GetCommandQueue()->Signal(fence.Get(), currentFenceValue);
		// fence�� 1�� �����
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to signal!");

		// 2. ���� �� ���� �ε��� ȹ�� (GPU�� �������� ��ģ ���۸� ������) Present���� ȣ���̹Ƿ� �ٲ�
		frameIndex = swapChain->GetCurrentBackBufferIndex();

		// 3. ���� �����ӿ� ���� �� ������ �潺 �� Ȯ�� �� ���
		 //    GPU�� ������ (�� 'frameIndex'�� �ش��ϴ� �� ���۸� ����ߴ�) �۾��� �Ϸ��ߴ��� Ȯ��
		if (fence->GetCompletedValue() < fenceValues[frameIndex]) // ���� ������ �ε���(���� ���ŵ�)�� �潺 ���� ��
		{
			// �Ϸ���� �ʾҴٸ�, �ش� �潺 ���� �ñ׳ε� ������ CPU ���
			fence->SetEventOnCompletion(fenceValues[frameIndex], fenceEvent.Get());
			WaitForSingleObjectEx(fenceEvent.Get(), INFINITE, FALSE);
		}
		fenceValues[frameIndex] = currentFenceValue + 1;
	}
}
