#include "DaydreamPCH.h"
#include "D3D12Texture.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"
#include "Daydream/Graphics/Utility/GraphicsUtil.h"

namespace Daydream
{
	D3D12Texture2D::D3D12Texture2D(D3D12RenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;

		if (GraphicsUtil::HasFlag(_desc.bindFlags, RenderBindFlags::RenderTarget))
		{
			texture = device->CreateTexture2D(width, height,
				GraphicsUtil::ConvertRenderFormatToDXGIFormat(_desc.format),
				GraphicsUtil::ConvertToD3D12BindFlags(_desc.bindFlags), D3D12_RESOURCE_STATE_RENDER_TARGET);
		}
		else if (GraphicsUtil::HasFlag(_desc.bindFlags, RenderBindFlags::DepthStencil))
		{
			texture = device->CreateTexture2D(width, height,
				GraphicsUtil::ConvertRenderFormatToDXGIFormat(_desc.format),
				GraphicsUtil::ConvertToD3D12BindFlags(_desc.bindFlags), D3D12_RESOURCE_STATE_DEPTH_WRITE);
		}
		else
		{
			texture = device->CreateTexture2D(width, height,
				GraphicsUtil::ConvertRenderFormatToDXGIFormat(_desc.format),
				GraphicsUtil::ConvertToD3D12BindFlags(_desc.bindFlags), D3D12_RESOURCE_STATE_COPY_DEST);

		}
			}
	

	D3D12Texture2D::~D3D12Texture2D()
	{
		if (rtvCpuHandle.ptr != 0)
		{
			device->GetRTVHeapAlloc().Free(rtvCpuHandle);
			rtvCpuHandle.ptr = 0;
		}
		if (dsvCpuHandle.ptr != 0)
		{
			device->GetDSVHeapAlloc().Free(dsvCpuHandle);
			rtvCpuHandle.ptr = 0;
		}
		if (srvCpuHandle.ptr != 0)
		{
			device->GetCBVSRVUAVHeapAlloc().Free(srvCpuHandle, srvGpuHandle);
			srvCpuHandle.ptr = 0;
			srvGpuHandle.ptr = 0;
		}
		if (uavCpuHandle.ptr != 0)
		{
			device->GetCBVSRVUAVHeapAlloc().Free(uavCpuHandle, uavGpuHandle);
			uavCpuHandle.ptr = 0;
			uavGpuHandle.ptr = 0;
		}
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
		DAYDREAM_CORE_ASSERT(srvCpuHandle.ptr == 0, "This texture was not created with the Shader Resourc View (SRV) bind flag.");
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
		DAYDREAM_CORE_ASSERT(rtvCpuHandle.ptr == 0, "This texture was not created with the Render Target View (RTV) bind flag.");
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
		DAYDREAM_CORE_ASSERT(false, "This texture was not created with the Depth Stencil View (DSV) bind flag.");
		return dsvCpuHandle;
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
		DAYDREAM_CORE_ASSERT(false, "This texture was not created with the Unordered Access View (UAV) bind flag.");
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
			//DAYDREAM_CORE_ASSERT(srvCpuHandle.ptr == 0, "This texture was not created with the Shader Resourc View (SRV) bind flag.");
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
