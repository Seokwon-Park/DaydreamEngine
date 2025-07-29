#include "SteinsPCH.h"
#include "D3D12Texture.h"

#include "Steins/Graphics/Utility/ImageLoader.h"
#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
	D3D12Texture2D::D3D12Texture2D(D3D12RenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;

		//texture
		D3D12_HEAP_PROPERTIES props{};
		props.Type = D3D12_HEAP_TYPE_DEFAULT;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC textureDesc{};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Alignment = 0;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = GraphicsUtil::ConvertRenderFormatToDXGIFormat(_desc.format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = GraphicsUtil::ConvertToD3D12BindFlags(_desc.bindFlags);

		device->GetDevice()->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(texture.GetAddressOf()));

	}
	D3D12Texture2D::D3D12Texture2D(D3D12RenderDevice* _device, const FilePath& _path, const TextureDesc& _desc)
		:Texture2D(_path)
	{
		device = _device;

		Array<UInt8> imageData = ImageLoader::LoadImageFile(_path, width, height, channels);

		//texture
		D3D12_HEAP_PROPERTIES props{};
		props.Type = D3D12_HEAP_TYPE_DEFAULT;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC textureDesc{};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Alignment = 0;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = GraphicsUtil::ConvertRenderFormatToDXGIFormat(_desc.format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = GraphicsUtil::ConvertToD3D12BindFlags(_desc.bindFlags);

		device->GetDevice()->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(texture.GetAddressOf()));

		D3D12_RESOURCE_DESC uploadBufferDesc{};
		D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedFootprint;
		UINT numRows;
		UINT64 rowSizeInBytes;
		UINT64 totalBytes;

		device->GetDevice()->GetCopyableFootprints(
			&textureDesc,
			0,
			1,
			0,
			&placedFootprint,
			&numRows,
			&rowSizeInBytes,
			&totalBytes
		);
		UInt32 uploadPitch = static_cast<UInt32>(placedFootprint.Footprint.RowPitch); // This will be aligned
		UInt32 uploadSize = static_cast<UInt32>(totalBytes); // Total bytes for the subresource
		//UInt32 uploadPitch = (width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
		//UInt32 uploadSize = height * uploadPitch;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		textureDesc.Width = uploadSize;
		textureDesc.Height = 1;
		textureDesc.Format = DXGI_FORMAT_UNKNOWN;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		props.Type = D3D12_HEAP_TYPE_UPLOAD;

		device->GetDevice()->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &textureDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf()));

		void* pixelData;
		D3D12_RANGE range = { 0, uploadSize };
		uploadBuffer->Map(0, &range, &pixelData);
		memcpy(pixelData, imageData.data(), width * height * 4);
		uploadBuffer->Unmap(0, &range);

		// 업로드 버퍼의 정보를 텍스쳐 처럼 해석하기 위해서
		D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
		srcLocation.pResource = uploadBuffer.Get();
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLocation.PlacedFootprint = placedFootprint;

		//이미 위에서 texture2d로 desc를 설정하고 만들었기 때문에 괜찮
		D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
		dstLocation.pResource = texture.Get();
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLocation.SubresourceIndex = 0;

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = texture.Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		device->GetCommandList()->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
		device->GetCommandList()->ResourceBarrier(1, &barrier);

		
	}
	D3D12Texture2D::D3D12Texture2D(D3D12RenderDevice* _device, ComPtr<ID3D12Resource> _texture)
	{
		device = _device;
		texture = _texture;
	}
	D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetSRVCPUHandle()
	{
		D3D12_RESOURCE_DESC textureDesc = texture->GetDesc();
		if (!(textureDesc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE))
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = textureDesc.Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			device->GetCBVSRVUAVHeapAlloc().Alloc(&srvCpuHandle, &srvGpuHandle);
			device->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, srvCpuHandle);
			return srvCpuHandle;
		}
		STEINS_CORE_ASSERT(srvCpuHandle.ptr == 0, "This texture was not created with the Shader Resourc View (SRV) bind flag.");
		return {};
	}
	D3D12_GPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetSRVGPUHandle()
	{
		if (srvGpuHandle.ptr == 0)
		{
			GetSRVCPUHandle();
		}
		return srvGpuHandle;
	}
	D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetRTVCPUHandle()
	{
		if (rtvCpuHandle.ptr != 0) return rtvCpuHandle;

		D3D12_RESOURCE_DESC textureDesc = texture->GetDesc();

		if (textureDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			device->GetRTVHeapAlloc().Alloc(&rtvCpuHandle);
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			rtvDesc.Format = textureDesc.Format;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			//// Determine ViewDimension based on texture type
			//if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && resDesc.DepthOrArraySize == 1) {
			//	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			//	// If MSAA: rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
			//}
			//else if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && resDesc.DepthOrArraySize > 1) {
			//	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
			//	rtvDesc.Texture2DArray.ArraySize = resDesc.DepthOrArraySize;
			//	rtvDesc.Texture2DArray.FirstArraySlice = 0;
			//	rtvDesc.Texture2DArray.MipSlice = 0; // Usually mip 0 for RTV
			//}
			// Handle 3D if needed, etc.

			device->GetDevice()->CreateRenderTargetView(texture.Get(), &rtvDesc, rtvCpuHandle);
			return rtvCpuHandle;
		}
		STEINS_CORE_ASSERT(rtvCpuHandle.ptr == 0, "This texture was not created with the Render Target View (RTV) bind flag.");
		return {};
	}
	const D3D12_CPU_DESCRIPTOR_HANDLE& D3D12Texture2D::GetDSVCPUHandle()
	{
		if (dsvCpuHandle.ptr != 0) return dsvCpuHandle;

		D3D12_RESOURCE_DESC textureDesc = texture->GetDesc();

		if (textureDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
		{
			device->GetDSVHeapAlloc().Alloc(&dsvCpuHandle); // DSVs only need CPU handle
			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
			dsvDesc.Format = textureDesc.Format;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE; // Can be D3D12_DSV_FLAG_READ_ONLY_DEPTH or _STENCIL
			dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

			// Determine ViewDimension based on texture type
			//if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && resDesc.DepthOrArraySize == 1) {
			//	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			//	// If MSAA: dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			//}
			//else if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && resDesc.DepthOrArraySize > 1) {
			//	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
			//	dsvDesc.Texture2DArray.ArraySize = resDesc.DepthOrArraySize;
			//	dsvDesc.Texture2DArray.FirstArraySlice = 0;
			//	dsvDesc.Texture2DArray.MipSlice = 0; // Usually mip 0 for DSV
			//}

			device->GetDevice()->CreateDepthStencilView(texture.Get(), &dsvDesc, dsvCpuHandle);
			return dsvCpuHandle;
		}
		STEINS_CORE_ASSERT(false, "This texture was not created with the Depth Stencil View (DSV) bind flag.");
		return {};
	}

	D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetUAVCPUHandle()
	{
		if (uavCpuHandle.ptr != 0) return uavCpuHandle;

		D3D12_RESOURCE_DESC textureDesc = texture->GetDesc();

		if (textureDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
		{
			D3D12_GPU_DESCRIPTOR_HANDLE tempGpuHandle;
			device->GetCBVSRVUAVHeapAlloc().Alloc(&uavCpuHandle, &tempGpuHandle); // UAV needs both CPU and GPU handle

			D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
			uavDesc.Format = textureDesc.Format;
			uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;

			// Determine ViewDimension based on texture type
			//if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && resDesc.DepthOrArraySize == 1) {
			//	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			//	uavDesc.Texture2D.MipSlice = 0; // Usually mip 0 for UAV
			//}
			//else if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D && resDesc.DepthOrArraySize > 1) {
			//	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
			//	uavDesc.Texture2DArray.ArraySize = resDesc.DepthOrArraySize;
			//	uavDesc.Texture2DArray.FirstArraySlice = 0;
			//	uavDesc.Texture2DArray.MipSlice = 0;
			//}
			//else if (resDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D) {
			//	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
			//	uavDesc.Texture3D.MipSlice = 0;
			//	uavDesc.Texture3D.FirstWSlice = 0;
			//	uavDesc.Texture3D.WSize = resDesc.DepthOrArraySize;
			//}

			device->GetDevice()->CreateUnorderedAccessView(texture.Get(), nullptr, &uavDesc, uavCpuHandle);
			return uavCpuHandle;
		}
		STEINS_CORE_ASSERT(false, "This texture was not created with the Unordered Access View (UAV) bind flag.");
		return {};
	}
	D3D12_GPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetUAVGPUHandle()
	{
		if (uavCpuHandle.ptr == 0)
		{
			GetUAVCPUHandle();
		}
		return uavGpuHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetSamplerCPUHandle()
	{
		if (samplerCpuHandle.ptr != 0) return samplerCpuHandle;
		else
		{
			D3D12_SAMPLER_DESC samplerDesc = {};
			samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 선형 필터링
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 텍스처 주소 모드 (반복)
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1; // 비등방성 필터링 사용 안 함
			samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 비교 함수 (그림자 맵 등에서 사용)
			samplerDesc.BorderColor[0] = 0.0f; // 경계 색상
			samplerDesc.BorderColor[1] = 0.0f;
			samplerDesc.BorderColor[2] = 0.0f;
			samplerDesc.BorderColor[3] = 0.0f;
			samplerDesc.MinLOD = 0.0f;
			samplerDesc.MaxLOD = D3D12_FLOAT32_MAX; // 모든 밉맵 레벨 사용

			device->GetSamplerHeapAlloc().Alloc(&samplerCpuHandle, &samplerGpuHandle);

			device->GetDevice()->CreateSampler(&samplerDesc, samplerCpuHandle);
			//STEINS_CORE_ASSERT(srvCpuHandle.ptr == 0, "This texture was not created with the Shader Resourc View (SRV) bind flag.");
		return samplerCpuHandle;
		}
	}

	D3D12_GPU_DESCRIPTOR_HANDLE D3D12Texture2D::GetSamplerGPUHandle()
	{
		if (samplerCpuHandle.ptr == 0)
		{
			GetSamplerCPUHandle();
		}
		return samplerGpuHandle;
	}

}
