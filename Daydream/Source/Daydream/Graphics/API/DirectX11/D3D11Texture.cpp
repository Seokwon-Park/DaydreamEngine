#include "DaydreamPCH.h"
#include "D3D11Texture.h"

#include "D3D11Sampler.h"
#include "D3D11Utility.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"

namespace Daydream
{
	D3D11Texture2D::D3D11Texture2D(D3D11RenderDevice* _device, const TextureDesc& _desc, const void* _initialData)
	{
		device = _device;

		width = _desc.width;
		height = _desc.height;

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = GraphicsUtility::DirectX11::ConvertToD3D11BindFlags(_desc.bindFlags);
		textureDesc.CPUAccessFlags = 0;

		HRESULT hr;
		if (_initialData)
		{
			D3D11_SUBRESOURCE_DATA pixelData{};
			pixelData.pSysMem = _initialData;
			pixelData.SysMemPitch = textureDesc.Width * sizeof(UInt8) * 4; //RGBA

			hr = device->GetDevice()->CreateTexture2D(&textureDesc, &pixelData, texture.GetAddressOf());
		}
		else
		{
			hr = device->GetDevice()->CreateTexture2D(&textureDesc, nullptr, texture.GetAddressOf());
		}
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Texture!");

		//D3D11_SAMPLER_DESC samplerDesc;
		//ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		//samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		//samplerDesc.MinLOD = 0;
		//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		//hr = device->GetDevice()->CreateSamplerState(&samplerDesc, textureSampler.GetAddressOf());
		//DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create Sampler!");

		if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			device->GetDevice()->CreateShaderResourceView(texture.Get(), nullptr, views.srv.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.srv, "This texture was not created with the Shader Resource View (SRV) bind flag.");
		}

		if (textureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		{
			device->GetDevice()->CreateDepthStencilView(texture.Get(), nullptr, views.dsv.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.dsv, "This texture was not created with the Depth Stencil View (DSV) bind flag.");
		}

		if (textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			device->GetDevice()->CreateRenderTargetView(texture.Get(), nullptr, views.rtv.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.rtv, "This texture was not created with the Render Target View (RTV) bind flag.");

		}

		if (textureDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			device->GetDevice()->CreateUnorderedAccessView(texture.Get(), nullptr, views.uav.GetAddressOf());
			DAYDREAM_CORE_ASSERT(views.uav, "This texture was not created with the Unordered Access View (UAV) bind flag.");
		}
	}

	D3D11Texture2D::~D3D11Texture2D()
	{
		device = nullptr;
		textureSampler = nullptr;
		texture.Reset();
		views.rtv.Reset();
		views.dsv.Reset();
		views.srv.Reset();
		views.uav.Reset();
	}


	void D3D11Texture2D::SetSampler(Shared<Sampler> _sampler)
	{
		textureSampler = static_cast<D3D11Sampler*>(_sampler.get());
	}
}
