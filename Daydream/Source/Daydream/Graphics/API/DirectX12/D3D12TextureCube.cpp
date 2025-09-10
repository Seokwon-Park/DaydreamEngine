#include "DaydreamPCH.h"
#include "D3D12TextureCube.h"

#include "D3D12Utility.h"

namespace Daydream
{
	D3D12TextureCube::D3D12TextureCube(D3D12RenderDevice* _device, const TextureDesc& _desc)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;

		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.DepthOrArraySize = 6;
		textureDesc.MipLevels = 1;
		textureDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		texture = device->CreateTexture(textureDesc, D3D12_RESOURCE_STATE_COPY_DEST);

		//D3D12_SAMPLER_DESC samplerDesc = {};
		//samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 선형 필터링
		//samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 텍스처 주소 모드 (반복)
		//samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		//samplerDesc.MipLODBias = 0.0f;
		//samplerDesc.MaxAnisotropy = 1; // 비등방성 필터링 사용 안 함
		//samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 비교 함수 (그림자 맵 등에서 사용)
		//samplerDesc.BorderColor[0] = 0.0f; // 경계 색상
		//samplerDesc.BorderColor[1] = 0.0f;
		//samplerDesc.BorderColor[2] = 0.0f;
		//samplerDesc.BorderColor[3] = 0.0f;
		//samplerDesc.MinLOD = 0.0f;
		//samplerDesc.MaxLOD = D3D12_FLOAT32_MAX; // 모든 밉맵 레벨 사용

		//device->GetSamplerHeapAlloc().Alloc(&samplerCpuHandle, &samplerGpuHandle);

		//device->GetDevice()->CreateSampler(&samplerDesc, samplerCpuHandle);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		device->GetCBVSRVUAVHeapAlloc().Alloc(&srvCpuHandle, &srvGpuHandle);
		device->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, srvCpuHandle);
	}
	D3D12TextureCube::~D3D12TextureCube()
	{

	}

	void D3D12TextureCube::SetSampler(Shared<Sampler> _sampler)
	{
		textureSampler = static_cast<D3D12Sampler*>(_sampler.get());
	}
}

