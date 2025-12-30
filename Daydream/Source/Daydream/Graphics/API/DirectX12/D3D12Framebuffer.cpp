#include "DaydreamPCH.h"
#include "D3D12Framebuffer.h"
#include "D3D12Swapchain.h"

namespace Daydream
{
	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, const FramebufferDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;
		renderPass = _renderPass;

		swapchainRTVHandle.ptr = 0;

		CreateAttachments();

		if (entityTexture)
		{
			ID3D12Resource* srcResource = entityTexture->GetID3D12Resource(); // 리소스 가져오기

			// 3. Readback Buffer 생성 (1픽셀 데이터만 담을 작은 버퍼)
			// 텍스처 복사 시 가로 정렬(Row Pitch) 제약이 있습니다 (D3D12_TEXTURE_DATA_PITCH_ALIGNMENT = 256 byte)

			D3D12_RESOURCE_DESC srcDesc = srcResource->GetDesc();
			// 1x1 영역에 대한 레이아웃 계산
			device->GetDevice()->GetCopyableFootprints(&srcDesc, 0, 1, 0, &footprint, nullptr, nullptr, &bufferSize);

			D3D12_HEAP_PROPERTIES heapProps = {};
			heapProps.Type = D3D12_HEAP_TYPE_READBACK;
			heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProps.CreationNodeMask = 1;
			heapProps.VisibleNodeMask = 1;

			D3D12_RESOURCE_DESC bufDesc = {};
			bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			bufDesc.Width = bufferSize; // 정렬된 크기
			bufDesc.Height = 1;
			bufDesc.DepthOrArraySize = 1;
			bufDesc.MipLevels = 1;
			bufDesc.Format = DXGI_FORMAT_UNKNOWN;
			bufDesc.SampleDesc.Count = 1;
			bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			bufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

			HRESULT hr = device->GetDevice()->CreateCommittedResource(
				&heapProps,
				D3D12_HEAP_FLAG_NONE,
				&bufDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(readTexture.GetAddressOf())
			);

			if (FAILED(hr)) DAYDREAM_CORE_WARN("Failed to create texture!");
		}
	}

	D3D12Framebuffer::D3D12Framebuffer(D3D12RenderDevice* _device, RenderPass* _renderPass, D3D12Swapchain* _swapChain, ID3D12Resource* _swapchainImage)
	{
		device = _device;
		auto desc = _swapchainImage->GetDesc();
		width = (UInt32)desc.Width;
		height = (UInt32)desc.Height;
		renderPass = _renderPass;

		D3D12_RESOURCE_DESC textureDesc = _swapchainImage->GetDesc();

		device->GetRTVHeapAlloc().Alloc(&swapchainRTVHandle);
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		device->GetDevice()->CreateRenderTargetView(_swapchainImage, &rtvDesc, swapchainRTVHandle);

		renderTargetHandles.push_back(swapchainRTVHandle);

		CreateAttachments();
	}

	D3D12Framebuffer::~D3D12Framebuffer()
	{
		if (swapchainRTVHandle.ptr != 0)
		{
			device->GetRTVHeapAlloc().Free(swapchainRTVHandle);
		}
		renderTargetHandles.clear();
		depthStencilHandle;
		colorAttachments.clear();
		depthAttachment = nullptr;
		device = nullptr;
	}

	Shared<Texture2D> D3D12Framebuffer::GetColorAttachmentTexture(UInt32 _index)
	{
		return colorAttachments[_index];
	}

	void D3D12Framebuffer::Resize(UInt32 _width, UInt32 _height)
	{
		width = _width;
		height = _height;

		oldAttachments.clear();
		oldAttachments = colorAttachments;
		colorAttachments.clear();
		oldAttachments.push_back(depthAttachment);
		depthAttachment = nullptr;
		renderTargetHandles.clear();
		depthStencilHandle.ptr = 0;

		CreateAttachments();
	}

	UInt32 D3D12Framebuffer::ReadEntityHandleFromPixel(Int32 _mouseX, Int32 _mouseY)
	{
		// 2. Entity ID가 저장된 텍스처 찾기
		Shared<D3D12Texture2D> targetTexture = entityTexture;
		if (!targetTexture) return 0;

		// 4. Command List 및 Command Allocator 확보 
		// (SingleTimeCommandList 처럼 즉시 실행 가능한 커맨드 리스트가 필요합니다)
		// 여기서는 D3D12RenderDevice에 관련 기능이 있다고 가정하고 작성합니다.
		// 없다면 직접 Allocator/List를 만들거나 기존 프레임 컨텍스트를 써야 합니다.
		//ID3D12GraphicsCommandList* cmdList = device->BeginSingleTimeCommands();

		device->ExecuteSingleTimeCommands([&](ID3D12GraphicsCommandList* _commandList)
			{
				// 5. Resource Barrier (RenderTarget -> CopySource)
				// 현재 상태가 무엇인지 추적해야 하지만, 보통 RT 상태일 것입니다.
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Transition.pResource = entityTexture->GetID3D12Resource();
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 혹은 COMMON/PIXEL_SHADER_RESOURCE
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				_commandList->ResourceBarrier(1, &barrier);

				// 6. 텍스처의 특정 픽셀(1x1)을 버퍼로 복사
				D3D12_TEXTURE_COPY_LOCATION dst = {};
				dst.pResource = readTexture.Get();
				dst.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
				dst.PlacedFootprint = footprint;

				D3D12_TEXTURE_COPY_LOCATION src = {};
				src.pResource = entityTexture->GetID3D12Resource();
				src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				src.SubresourceIndex = 0;

				D3D12_BOX srcBox = {};
				srcBox.left = _mouseX;
				srcBox.top = _mouseY;
				srcBox.front = 0;
				srcBox.right = _mouseX + 1;
				srcBox.bottom = _mouseY + 1;
				srcBox.back = 1;

				_commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, &srcBox);

				// 7. Resource Barrier 복구 (CopySource -> RenderTarget)
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 원래 상태로 복구
				_commandList->ResourceBarrier(1, &barrier);
			});

		// 9. 데이터 읽기 (Map)
		UInt32 pixelValue = 0;
		void* mappedData = nullptr;
		D3D12_RANGE readRange = { 0, bufferSize }; // 읽을 범위

		readTexture->Map(0, &readRange, &mappedData);
		// 텍스처 데이터는 RowPitch에 맞춰 정렬되어 있으므로, 
		// 첫 번째 픽셀 데이터(우리는 1x1만 복사했으므로 시작점)를 읽으면 됩니다.
		// R32_UINT 포맷이므로 4바이트(UInt32)로 읽습니다.
		pixelValue = *((UInt32*)mappedData);

		D3D12_RANGE writeRange = { 0, 0 }; // 쓸 데이터 없음
		readTexture->Unmap(0, &writeRange);

		return pixelValue;
	}

	void D3D12Framebuffer::CreateAttachments()
	{
		const RenderPassDesc& renderPassDesc = renderPass->GetDesc();
		for (const auto& colorAttachmentDesc : renderPassDesc.colorAttachments)
		{
			if (colorAttachmentDesc.isSwapchain) continue;
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = colorAttachmentDesc.format;
			textureDesc.bindFlags = RenderBindFlags::RenderTarget | RenderBindFlags::ShaderResource;

			Shared<D3D12Texture2D> colorTexture = MakeShared<D3D12Texture2D>(device, textureDesc);
			if (colorAttachmentDesc.type == AttachmentType::EntityHandle)
			{
				entityTexture = colorTexture;
			}
			colorAttachments.push_back(colorTexture);
			renderTargetHandles.push_back(colorTexture->GetRTVCPUHandle());
		}

		if (renderPassDesc.depthAttachment.format != RenderFormat::UNKNOWN)
		{
			TextureDesc textureDesc;
			textureDesc.width = width;
			textureDesc.height = height;
			textureDesc.format = renderPassDesc.depthAttachment.format;
			textureDesc.bindFlags = RenderBindFlags::DepthStencil | RenderBindFlags::ShaderResource;

			Shared<D3D12Texture2D> depthTexture = MakeShared<D3D12Texture2D>(device, textureDesc);
			depthAttachment = depthTexture;
			depthStencilHandle = depthTexture->GetDSVCPUHandle();
		}
	}
}