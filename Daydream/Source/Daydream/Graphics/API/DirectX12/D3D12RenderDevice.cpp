#include "DaydreamPCH.h"
#include "D3D12RenderDevice.h"
#include "D3D12Buffer.h"
#include "D3D12RenderContext.h"
#include "D3D12RenderPass.h"
#include "D3D12Framebuffer.h"
#include "D3D12Shader.h"
#include "D3D12Swapchain.h"
#include "D3D12PipelineState.h"
#include "D3D12ImGuiRenderer.h"
#include "D3D12Texture.h"
#include "D3D12TextureCube.h"
#include "D3D12Sampler.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"

namespace Daydream
{
	D3D12RenderDevice::D3D12RenderDevice()
	{
		API = RendererAPIType::DirectX12;
	}

	D3D12RenderDevice::~D3D12RenderDevice()
	{

	}

	void D3D12RenderDevice::Init()
	{
		UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
		// Enable the D3D12 debug layer.
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()))))
		{
			debugLayer->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif
		// DXGI ���丮 ����
		HRESULT hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(dxgiFactory.GetAddressOf()));
		if (FAILED(hr))
		{
			DAYDREAM_CORE_ERROR("Failed to create dxgiFactory!");
		}

		// ����̽� ����
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

			//�� ����ͷ� Device�� ���� �� �ִ��� �׽�Ʈ�Ѵ�.
			hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
			{
				//�����ϸ� �������´�.
				isAdapterFound = true;
				break;
			}
		}

		DAYDREAM_CORE_ASSERT(isAdapterFound, "Failed to find Adapter!");

		hr = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(device.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to Create Device!");

#if defined(DEBUG) || defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
		if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
		{
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE); // ���ϸ� TRUE��

			// �ʿ� ���� �޽����� ���͸��� ����
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

		// Ŀ�ǵ� ť ����
		{
			D3D12_COMMAND_QUEUE_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			device->CreateCommandQueue(&desc, IID_PPV_ARGS(commandQueue.GetAddressOf()));
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.NumDescriptors = 8;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(rtvHeap.GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create RTV descriptor heap");
			rtvHeapAlloc.Create(device.Get(), rtvHeap.Get());
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.NumDescriptors = 4;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(dsvHeap.GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create DSV descriptor heap");
			dsvHeapAlloc.Create(device.Get(), dsvHeap.Get());

		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.NumDescriptors = 512;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(samplerHeap.GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create sampler descriptor heap");
			samplerHeapAlloc.Create(device.Get(), samplerHeap.Get());

		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.NumDescriptors = 1024;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(cbvSrvUavHeap.GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create cbvSrvUav descriptor heap");
			cbvSrvUavHeapAlloc.Create(device.Get(), cbvSrvUavHeap.Get());
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.NumDescriptors = 512;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(dynamicSamplerHeap.GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create sampler descriptor heap");
			dynamicSamplerHeapAlloc.Create(device.Get(), dynamicSamplerHeap.Get());

		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc{};
			desc.NumDescriptors = 1024;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(dynamicCbvSrvUavHeap.GetAddressOf()));
			DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create cbvSrvUav descriptor heap");
			dynamicCbvSrvUavHeapAlloc.Create(device.Get(), dynamicCbvSrvUavHeap.Get());
		}

		hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(uploadCommandAllocator.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create command allocator");

		hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, uploadCommandAllocator.Get(), nullptr, IID_PPV_ARGS(uploadCommandList.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create commandlist");

		uploadCommandList->Close();

		hr = device->CreateFence(uploadFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(uploadFence.GetAddressOf()));
		//���� �潺���� 1�� ��������
		uploadFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (uploadFenceEvent == nullptr)
		{
			DAYDREAM_CORE_ERROR("Failed to Create FenceEvent!");
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

		DAYDREAM_CORE_INFO("DirectX12 Info:");
		DAYDREAM_CORE_INFO("  Vendor: {0}", GraphicsUtility::GetVendor(adapterDescription.VendorId));
		DAYDREAM_CORE_INFO("  Renderer: {0} {1} GB", videoCardDescription, std::round((double)adapterDescription.DedicatedVideoMemory / (1 << 30)));
		DAYDREAM_CORE_INFO("  Version: {0}", version);

		//device->CreateDescriptorHeap()
		//// ���� ü�� ����
		//DXGI_SWAP_CHAIN_DESC swapDesc = {};
		//swapDesc.BufferCount = 2;
		//swapDesc.BufferDesc.Width = 800;
		//swapDesc.BufferDesc.Height = 600;
		//swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//swapDesc.OutputWindow = hwnd;
		//swapDesc.Windowed = TRUE;
		//swapDesc.SampleDesc.Count = 1;
		//dxgiFactory->CreateSwapchain(commandQueue.Get(), &swapDesc, &swapChain);

	}

	void D3D12RenderDevice::Shutdown()
	{
	}

	void D3D12RenderDevice::Render()
	{
	}

	Shared<RenderContext> D3D12RenderDevice::CreateContext()
	{
		return MakeShared<D3D12GraphicsContext>(this);
	}

	Shared<VertexBuffer> D3D12RenderDevice::CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize, const void* _initialData)
	{
		auto vertexBuffer = MakeShared<D3D12VertexBuffer>(this, BufferUsage::Dynamic, _size, _stride);

		vertexBuffer->SetData(_initialData, _initialDataSize);

		return vertexBuffer;
	}

	Shared<VertexBuffer> D3D12RenderDevice::CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData)
	{
		auto vertexBuffer = MakeShared<D3D12VertexBuffer>(this, BufferUsage::Static, _size, _stride);

		auto uploadBuffer = CreateBuffer(_size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

		void* bufferData;
		HRESULT hr = uploadBuffer->Map(0, nullptr, &bufferData);
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to map uploadBuffer");
		memcpy(bufferData, _initialData, _size);
		uploadBuffer->Unmap(0, nullptr);

		CopyBuffer(uploadBuffer.Get(), vertexBuffer->GetDX12Buffer(), _size);

		TransitionResourceState(vertexBuffer->GetDX12Buffer(), D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		return vertexBuffer;
	}


	Shared<IndexBuffer> Daydream::D3D12RenderDevice::CreateIndexBuffer(const UInt32* _indices, UInt32 _count)
	{
		auto indexBuffer = MakeShared<D3D12IndexBuffer>(this, _count);

		UInt32 bufferSize = sizeof(UInt32) * _count;
		auto uploadBuffer = CreateBuffer(bufferSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

		void* bufferData;
		HRESULT hr = uploadBuffer->Map(0, nullptr, &bufferData);
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to map uploadBuffer");
		memcpy(bufferData, _indices, bufferSize);
		uploadBuffer->Unmap(0, nullptr);

		CopyBuffer(uploadBuffer.Get(), indexBuffer->GetDX12Buffer(), bufferSize);

		TransitionResourceState(indexBuffer->GetDX12Buffer(), D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_INDEX_BUFFER);

		return indexBuffer;
	}

	Shared<RenderPass> D3D12RenderDevice::CreateRenderPass(const RenderPassDesc& _desc)
	{
		return MakeShared<D3D12RenderPass>(this, _desc);
	}

	Shared<Framebuffer> D3D12RenderDevice::CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return _renderPass->CreateFramebuffer(_desc);
	}

	Shared<PipelineState> D3D12RenderDevice::CreatePipelineState(const PipelineStateDesc& _desc)
	{
		return MakeShared<D3D12PipelineState>(this, _desc);
	}

	Shared<Shader> D3D12RenderDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return MakeShared<D3D12Shader>(this, _src, _type, _mode);
	}

	Shared<Swapchain> D3D12RenderDevice::CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		return MakeShared<D3D12Swapchain>(this, _window, _desc);
	}

	Shared<Texture2D> D3D12RenderDevice::CreateTexture2D(const void* _imageData, const TextureDesc& _desc)
	{
		Shared<D3D12Texture2D> texture = MakeShared<D3D12Texture2D>(this, _desc);

		if (_imageData != nullptr)
		{
			D3D12_RESOURCE_DESC dstDesc = texture->GetID3D12Resource()->GetDesc();

			D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedFootprint;
			UINT64 totalBytes;
			device->GetCopyableFootprints(&dstDesc, 0, 1, 0, &placedFootprint, nullptr, nullptr, &totalBytes);

			auto uploadBuffer = CreateBuffer(totalBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

			UInt32 imageSize = _desc.width * _desc.height * 4;

			void* pixelData;
			D3D12_RANGE range = { 0, totalBytes };
			uploadBuffer->Map(0, &range, &pixelData);
			// 3. �� �پ� ������ ���� �����մϴ�.
			BYTE* pDest = static_cast<BYTE*>(pixelData);
			BYTE* pSrc = const_cast<BYTE*>(static_cast<const BYTE*>(_imageData)); // ���� �̹��� ������
			UINT rowCount = dstDesc.Height;
			UINT rowSizeInBytes = dstDesc.Width * 4; // �ȼ��� 4����Ʈ ����

			for (UINT y = 0; y < rowCount; ++y)
			{
				// �� ��(RowSizeInBytes) ��ŭ�� ����
				memcpy(pDest, pSrc, rowSizeInBytes);

				// ���� �ٷ� ������ �̵�
				pDest += placedFootprint.Footprint.RowPitch; // �������� RowPitch��ŭ �̵�
				pSrc += rowSizeInBytes;            // �ҽ��� ���� ������ ũ�⸸ŭ �̵�
			}

			uploadBuffer->Unmap(0, &range);

			uploadBuffer->SetName(L"Check");

			CopyBufferToImage(uploadBuffer.Get(), texture->GetID3D12Resource(), { placedFootprint });
		}
		else
		{
			ExecuteSingleTimeCommands(
				[&](ID3D12GraphicsCommandList* _commandList)
				{
					TransitionResourceState(texture->GetID3D12Resource(), D3D12_RESOURCE_STATE_COPY_DEST,
						D3D12_RESOURCE_STATE_COMMON);
				}
			);
		}


		return texture;
	}

	Shared<TextureCube> D3D12RenderDevice::CreateTextureCube(Array<const void*>& _imagePixels, const TextureDesc& _desc)
	{
		auto texture = MakeShared<D3D12TextureCube>(this, _desc);

		UInt32 imageSize = _desc.width * _desc.height * 4 * 6;
		auto uploadBuffer = CreateBuffer(imageSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

		D3D12_RESOURCE_DESC textureDesc = texture->GetID3D12Resource()->GetDesc();
		std::vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> layouts(6);
		UINT64 totalBytes;
		device->GetCopyableFootprints(&textureDesc, 0, 6, 0, layouts.data(), nullptr, nullptr, &totalBytes);

		UInt8* pixelData;
		uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pixelData));
		for (int i = 0; i < 6; ++i)
		{
			const void* srcPixels = _imagePixels[i]; // i��° �̹��� ������ ������
			const auto& destLayout = layouts[i];
			UINT rowPitch = _desc.width * 4; // �ȼ��� 4����Ʈ(R8G8B8A8)��� ����

			// �޸� ���̾ƿ�(row pitch)�� ����Ͽ� �� �پ� ����
			for (UINT y = 0; y < _desc.height; ++y)
			{
				// ������ �ּ� = ���� ���� �ּ� + ���� ���� ������ + ���� y ��ġ ������
				UInt8* dest = pixelData + destLayout.Offset + y * destLayout.Footprint.RowPitch;
				// �ҽ� �ּ� = i��° �̹��� ������ ���� �ּ� + ���� y ��ġ ������
				const UINT8* src = static_cast<const UINT8*>(srcPixels) + y * rowPitch;

				memcpy(dest, src, rowPitch);
			}
		}
		uploadBuffer->Unmap(0, nullptr);


		CopyBufferToImage(uploadBuffer.Get(), texture->GetID3D12Resource(), layouts);
		//ExecuteSingleTimeCommands(
		//	[&](ID3D12GraphicsCommandList* _commandList)
		//	{
		//		TransitionResourceState(texture->GetID3D12Resource(), D3D12_RESOURCE_STATE_COPY_DEST,
		//			D3D12_RESOURCE_STATE_COMMON);
		//	}
		//);

		return texture;
	}

	Shared<Sampler> D3D12RenderDevice::CreateSampler(const SamplerDesc& _desc)
	{
		return MakeShared<D3D12Sampler>(this, _desc);
	}

	Unique<ImGuiRenderer> D3D12RenderDevice::CreateImGuiRenderer()
	{
		return MakeUnique<D3D12ImGuiRenderer>(this);
	}

	Shared<ConstantBuffer> D3D12RenderDevice::CreateConstantBuffer(UInt32 _size)
	{
		return MakeShared<D3D12ConstantBuffer>(this, _size);
	}

	Shared<Material> D3D12RenderDevice::CreateMaterial(Shared<PipelineState> _pipeline)
	{
		return _pipeline->CreateMaterial();
	}

	void D3D12RenderDevice::ExecuteSingleTimeCommands(std::function<void(ID3D12GraphicsCommandList*)> commands)
	{
		// 1. Allocator�� Command List�� �����ϱ� ���� ����
		uploadCommandAllocator->Reset();
		uploadCommandList->Reset(uploadCommandAllocator.Get(), nullptr);

		// 2. �ܺο��� ���޹��� Ŀ�ǵ� ��� (���� �Լ� ���)
		commands(uploadCommandList.Get());

		// 3. Ŀ�ǵ� ����Ʈ ��� ����
		uploadCommandList->Close();

		// 4. Ŀ�ǵ� ����Ʈ�� ť�� �����Ͽ� ����
		ID3D12CommandList* ppCommandLists[] = { uploadCommandList.Get() };
		commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// 5. Fence�� ����Ͽ� �۾��� �Ϸ�� ������ ���
		uploadFenceValue++;
		commandQueue->Signal(uploadFence.Get(), uploadFenceValue);

		if (uploadFence->GetCompletedValue() < uploadFenceValue)
		{
			uploadFence->SetEventOnCompletion(uploadFenceValue, uploadFenceEvent);
			WaitForSingleObject(uploadFenceEvent, INFINITE);
		}
	}
	ComPtr<ID3D12Resource> D3D12RenderDevice::CreateBuffer(UINT64 _size, D3D12_HEAP_TYPE _heapType, D3D12_RESOURCE_STATES _initialState, D3D12_RESOURCE_FLAGS _flags)
	{
		ComPtr<ID3D12Resource> buffer;

		// 1. ��(Heap)�� �Ӽ��� �����մϴ�. VMA�� MemoryUsage�� �ش��մϴ�.
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = _heapType;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		// 2. ���ҽ�(����)�� �Ӽ��� �����մϴ�.
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = _size;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = _flags;

		HRESULT hr = device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			_initialState,
			nullptr, // Clear value (���ۿ��� �ʿ� ����)
			IID_PPV_ARGS(buffer.GetAddressOf()));

		return buffer;
	}

	void D3D12RenderDevice::CopyBuffer(ID3D12Resource* _src, ID3D12Resource* _dst, UInt32 _dataSize)
	{
		ExecuteSingleTimeCommands([&](ID3D12GraphicsCommandList* _commandList)
			{
				_commandList->CopyBufferRegion(
					_dst,
					0,
					_src,
					0,
					_dataSize
				);
			});
	}

	ComPtr<ID3D12Resource> D3D12RenderDevice::CreateTexture(const D3D12_RESOURCE_DESC& _desc, D3D12_RESOURCE_STATES _initialState)
	{
		ComPtr<ID3D12Resource> texture;

		D3D12_CLEAR_VALUE clearValue{};
		bool isUseClearValue = _desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET || _desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		if (isUseClearValue)
		{
			clearValue.Format = _desc.Format;
			if (_desc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
			{
				clearValue.DepthStencil.Depth = 1.0f;
				clearValue.DepthStencil.Stencil = 0;
			}
			else
			{
				clearValue.Color[0] = 0.0f;
				clearValue.Color[1] = 0.0f;
				clearValue.Color[2] = 1.0f;
				clearValue.Color[3] = 1.0f;
			}
		}

		// 2. �ؽ�ó�� ��κ� GPU ���� �޸�(DEFAULT ��)�� �����˴ϴ�.
		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

		// 3. ���ҽ� ������ �޸� �Ҵ��� �� ���� ó���մϴ�.
		device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&_desc,
			_initialState,
			isUseClearValue ? &clearValue : nullptr,
			IID_PPV_ARGS(texture.GetAddressOf())
		);
		return texture;
	}


	void Daydream::D3D12RenderDevice::CopyBufferToImage(ID3D12Resource* _src, ID3D12Resource* _dst, Array<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> _subresourceFootprint)
	{
		ExecuteSingleTimeCommands([&](ID3D12GraphicsCommandList* _commandList)
			{
				for (UInt32 i = 0; i < _subresourceFootprint.size(); ++i)
				{
					D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
					srcLocation.pResource = _src;
					srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
					srcLocation.PlacedFootprint = _subresourceFootprint[i];

					D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
					dstLocation.pResource = _dst;
					dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
					dstLocation.SubresourceIndex = i;

					_commandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);
				}

				TransitionResourceState(_dst, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			});
	}

	void D3D12RenderDevice::TransitionResourceState(ID3D12Resource* _resource, D3D12_RESOURCE_STATES _stateBefore, D3D12_RESOURCE_STATES _stateAfter)
	{
		if (_stateBefore == _stateAfter)
		{
			return;
		}

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = _resource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = _stateBefore;
		barrier.Transition.StateAfter = _stateAfter;

		commandList->ResourceBarrier(1, &barrier);
	}


	//void D3D12RenderDevice::WaitForGPU(IDXGISwapChain3* _swapChain)
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


